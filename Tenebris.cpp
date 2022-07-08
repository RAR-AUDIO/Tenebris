//==================================================================================
/* iPlug Includes*/
#include "Tenebris.h"

#include "IBubbleControl.h"
#include "IPlug_include_in_plug_src.h"
//==================================================================================
/* RarClasses Includes*/
#include "Utils/RAR_FontAwesome.hpp"
#include "Utils/RAR_Graphics.hpp"
#include "Utils/Controls/SettingsMenu.hpp"

//===========================================================s=======================

PLUG_CLASS_NAME::PLUG_CLASS_NAME (const InstanceInfo& info)
    : Plugin (info, MakeConfig (KNumParams, K_NUM_PRESETS))
{
    GetParam (KBits)->InitInt ("Bits", 24, 1, 24, "Bit");
    GetParam (KSampleRate)->InitFrequency ("Sample Rate", GetSampleRate(), 40.0, GetSampleRate(), 1.0);
    GetParam (KInputGain)->InitDouble ("Input Gain", 100.0f, 0.f, 200.0f, 1.0f, "%");
    GetParam (KMix)->InitDouble ("Dry/Wet", 100.0f, 0.0f, 100.0f, 1.0f, "%");
    GetParam (KOutputGain)->InitDouble ("Output Gain", 100.0f, 0.0f, 200.0f, 1.0f, "%");
    GetParam (KClippingEnabled)->InitBool ("Clipping", false, "", IParam::kFlagsNone, "", "Disabled", "Enabled");
    GetParam (KIsActive)->InitBool ("Active", true, "", IParam::kFlagsNone, "", "Disabled", "Enabled");
    GetParam (KLpActive)->InitBool ("PostFilter Active", false, "", IParam::kFlagsNone, "", "Disabled", "Enabled");
    GetParam (KHpActive)->InitBool ("HighPassFilter Active", false, "", IParam::kFlagsNone, "", "Disabled", "Enabled");

#if IPLUG_EDITOR // http://bit.ly/2S64BDd
    mMakeGraphicsFunc = [&]()
    { return MakeGraphics (*this, PLUG_WIDTH, PLUG_HEIGHT, PLUG_FPS, GetScaleForScreen (PLUG_WIDTH, PLUG_HEIGHT)); };

    mLayoutFunc = [&] (IGraphics* pGraphics)
    {
    /* IPlug 2 Specific Controls */

#if RAR_DEBUG
        pGraphics->AttachCornerResizer (EUIResizerMode::Scale,
                                        false,
                                        IColor (20, 127, 127, 127),
                                        COLOR_WHITE,
                                        COLOR_BLACK);
#endif

        pGraphics->EnableTooltips (true);
        pGraphics->EnableMouseOver (true);
        pGraphics->EnableMultiTouch (true);

        pGraphics->AttachBubbleControl (new IBubbleControl (IText (14.0F,
                                                                   EAlign::Center,
                                                                   IColor (255, 254, 245, 245)),
                                                            IColor (255, 20, 20, 20),
                                                            IColor (255, 15, 15, 15)));

        /* Loading Svg */
        const auto screw = pGraphics->LoadSVG (BlackScrew_FN);
        const auto bezel = pGraphics->LoadSVG (LedBezel_FN);

        /* Loading Fonts */
        pGraphics->LoadFont ("Roboto-Regular", ROBOTO_FN);
        pGraphics->LoadFont ("IBMPlexSans", IBM_PLEX_SANS_FN);
        pGraphics->LoadFont ("IBMPlexSans-Thin", IBM_PLEX_SANS_THIN_FN);
        pGraphics->LoadFont ("fontaudio", FONTAUDIO_FN);
        pGraphics->LoadFont ("forkAwesome", FORK_AWESOME_FN);

        /* Main Styles */
        const rar::graphics::layout::RarStyle defaultKnob {
            true, // label
            true, // value
            {
                // color spec:
                COLOR_TRANSPARENT, // Background
                IColor (255, 29, 31, 32), // Foreground
                rar::graphics::layout::RAR_COLOR_DARK_GRAY, // Pressed
                rar::graphics::layout::RAR_COLOR_BLACK, // Frame
                DEFAULT_HLCOLOR, // Highlight
                rar::graphics::layout::RAR_COLOR_BLACK_DROP_SHADOW, // Shadow
                rar::graphics::layout::RAR_COLOR_DARK_GRAY, // Extra 1
                IColor (255, 245, 245, 245), // Extra 2
                DEFAULT_X3COLOR // Extra 3
            },
            {
                // label text:
                DEFAULT_TEXT_SIZE, // size
                rar::graphics::layout::RAR_COLOR_WORNOUT_WHITE, // color
                "IBMPlexSans", // font
                EAlign::Center, // horizontal align
                EVAlign::Top, // vertical align
                0 // angle
            },
            {
                // value text:
                DEFAULT_TEXT_SIZE, // size
                rar::graphics::layout::RAR_COLOR_WORNOUT_WHITE, // color
                "IBMPlexSans", // font
                EAlign::Center, // horizontal align
                EVAlign::Bottom, // vertical align
                0, // angle
                DEFAULT_TEXTENTRY_BGCOLOR, // text entry BG color
                DEFAULT_TEXTENTRY_FGCOLOR // text entry text color
            },
            DEFAULT_HIDE_CURSOR, // hide cursor
            DEFAULT_DRAW_FRAME, // draw frame
            true, // draw shadows
            DEFAULT_EMBOSS, // emboss
            0.1f, // roundness
            2.f, // frame thickness
            3.5f, // shadow offset
            DEFAULT_WIDGET_FRAC, // widget fraction
            DEFAULT_WIDGET_ANGLE // widget angle
        };

        
        /* blue knob */
        const rar::graphics::layout::RarStyle blueKnob {
            true, // label
            true, // value
            {
                // color spec:
                COLOR_TRANSPARENT, // Background
                IColor (255, 30, 60, 160), // Foreground
                IColor (255, 19, 117, 168), // Pressed
                rar::graphics::layout::RAR_COLOR_BLACK, // Frame
                DEFAULT_HLCOLOR, // Highlight
                rar::graphics::layout::RAR_COLOR_BLACK_DROP_SHADOW, // Shadow
                IColor (255, 19, 117, 168), // Extra 1
                IColor (255, 245, 245, 245), // Extra 2
                DEFAULT_X3COLOR // Extra 3
            },
            {
                // label text:
                DEFAULT_TEXT_SIZE, // size
                rar::graphics::layout::RAR_COLOR_WORNOUT_WHITE, // color
                "IBMPlexSans", // font
                EAlign::Center, // horizontal align
                EVAlign::Top, // vertical align
                0 // angle
            },
            {
                // value text:
                DEFAULT_TEXT_SIZE, // size
                rar::graphics::layout::RAR_COLOR_WORNOUT_WHITE, // color
                "IBMPlexSans", // font
                EAlign::Center, // horizontal align
                EVAlign::Bottom, // vertical align
                0, // angle
                DEFAULT_TEXTENTRY_BGCOLOR, // text entry BG color
                DEFAULT_TEXTENTRY_FGCOLOR // text entry text color
            },
            DEFAULT_HIDE_CURSOR, // hide cursor
            DEFAULT_DRAW_FRAME, // draw frame
            true, // draw shadows
            DEFAULT_EMBOSS, // emboss
            0.1f, // roundness
            2.f, // frame thickness
            3.5f, // shadow offset
            DEFAULT_WIDGET_FRAC, // widget fraction
            DEFAULT_WIDGET_ANGLE // widget angle
        };

        /* red knob */
        const rar::graphics::layout::RarStyle redKnob {
            true, // label
            true, // value
            {
                // color spec:
                COLOR_TRANSPARENT, // Background
                IColor (255, 128, 18, 18), // Foreground
                IColor (255, 229, 46, 47), // Pressed
                rar::graphics::layout::RAR_COLOR_BLACK, // Frame
                DEFAULT_HLCOLOR, // Highlight
                rar::graphics::layout::RAR_COLOR_BLACK_DROP_SHADOW, // Shadow
                IColor (255, 229, 46, 47), // Extra 1
                IColor (255, 245, 245, 245), // Extra 2
                DEFAULT_X3COLOR // Extra 3
            },
            {
                // label text:
                DEFAULT_TEXT_SIZE, // size
                rar::graphics::layout::RAR_COLOR_WORNOUT_WHITE, // color
                "IBMPlexSans", // font
                EAlign::Center, // horizontal align
                EVAlign::Top, // vertical align
                0 // angle
            },
            {
                // value text:
                DEFAULT_TEXT_SIZE, // size
                rar::graphics::layout::RAR_COLOR_WORNOUT_WHITE, // color
                "IBMPlexSans", // font
                EAlign::Center, // horizontal align
                EVAlign::Bottom, // vertical align
                0, // angle
                DEFAULT_TEXTENTRY_BGCOLOR, // text entry BG color
                DEFAULT_TEXTENTRY_FGCOLOR // text entry text color
            },
            DEFAULT_HIDE_CURSOR, // hide cursor
            DEFAULT_DRAW_FRAME, // draw frame
            true, // draw shadows
            DEFAULT_EMBOSS, // emboss
            0.1f, // roundness
            2.f, // frame thickness
            3.5f, // shadow offset
            DEFAULT_WIDGET_FRAC, // widget fraction
            DEFAULT_WIDGET_ANGLE // widget angle

        };

        /* Button Style */
        const rar::graphics::layout::RarStyle buttonStyle = rar::graphics::layout::RAR_DEFAULT_STYLE.withColor (kFG, IColor (255, 81, 81, 81)).withColor (kPR, IColor (255, 108, 108, 108)).withShowLabel (false).withShowValue (false).withDrawShadows (true).withWidgetFrac (0.8);

        const IVStyle buttonStyleIV = DEFAULT_STYLE.WithColor (kFG, IColor (255, 81, 81, 81)).WithColor (kPR, IColor (255, 108, 108, 108)).WithShowLabel (false).WithShowValue (false).WithDrawShadows (true).WithWidgetFrac (0.8);

        /* forkAwesome*/
        // for topbar menu (bars) with other stuff WIP/Not Yet Created.

        /* Main Controls */
        const auto b = pGraphics->GetBounds(); // background bounds
        const auto barTop = b.GetFromTop (50.0F); // top bar bounds
        const auto barBottom = b.GetFromBottom (20.0F); // bottom bar bounds

        pGraphics->AttachPanelBackground (IColor (255, 20, 20, 20)); // background color

#pragma region TopBar
        pGraphics->AttachControl (new rar::graphics::controls::RarPanel (barTop, IColor (255, 17, 17, 17), false));

        pGraphics->AttachControl (new ITextControl (IRECT (20.0F, 0, 240.0F, 50.0F),
                                                    "TENEBRIS",
                                                    IText (48.0F,
                                                           rar::graphics::layout::RAR_COLOR_WHITE_SMOKE,
                                                           "IBMPlexSans",
                                                           EAlign::Near,
                                                           EVAlign::Bottom),
                                                    DEFAULT_BGCOLOR,
                                                    false));


        /* Preset Menu  *WIP* */

        /* PowerButton */
        pGraphics->AttachControl (new rar::graphics::controls::RarTextToggleControl (barTop.GetFromRight (40.0F).GetCentredInside (40.0F, 30.0F),
                                                                                     KIsActive,
                                                                                     ICON_FAD_POWERSWITCH,
                                                                                     ICON_FAD_POWERSWITCH,
                                                                                     IText (24.0F,
                                                                                            rar::graphics::layout::RAR_COLOR_GREEN.WithContrast (0.3F),
                                                                                            "fontaudio"),
                                                                                     IText (24.0F,
                                                                                            rar::graphics::layout::RAR_COLOR_RED,
                                                                                            "fontaudio"),
                                                                                     DEFAULT_BGCOLOR));

        pGraphics->AttachControl (new rar::graphics::display::SettingsMenu (barTop.GetFromRight (100.0f).GetCentredInside (24.0f)));

#pragma endregion TopBar

#pragma region Screws
        //Top Left
        pGraphics->AttachControl (new ISVGControl (b.GetFromLeft (42.f).GetFromTop (117.f).GetCentredInside (16.f, 16.f),
                                                   screw,
                                                   false));
        //Top Right
        pGraphics->AttachControl (new ISVGControl (b.GetFromRight (42.f).GetFromTop (117.f).GetCentredInside (16.f, 16.f),
                                                   screw,
                                                   false));
        //Bottom Left
        pGraphics->AttachControl (new ISVGControl (b.GetFromLeft (42.f).GetFromBottom (80.f).GetCentredInside (16.f, 16.f),
                                                   screw,
                                                   false));
        //Bottom Right
        pGraphics->AttachControl (new ISVGControl (b.GetFromRight (42.f).GetFromBottom (80.f).GetCentredInside (16.f, 16.f),
                                                   screw,
                                                   false));

#pragma endregion Screws

#pragma region Middle

        // Status LEDs
        pGraphics->AttachControl (new rar::graphics::controls::RarLedControl (b.GetReducedFromTop (90).GetFromTop (14).GetReducedFromRight (20).GetFromRight (20),
                                                                              COLOR_GREEN.WithContrast (0.8F),
                                                                              KIsActive),
                                  CSignalInLed);

        pGraphics->AttachControl (new ITextControl (b.GetReducedFromTop (90).GetFromTop (14).GetReducedFromRight (45).GetFromRight (39),
                                                    "Signal In",
                                                    IText (14,
                                                           rar::graphics::layout::RAR_COLOR_WORNOUT_WHITE,
                                                           "IBMPlexSans")));

        pGraphics->AttachControl (new ILEDControl (b.GetReducedFromTop (130).GetFromTop (14).GetReducedFromRight (20).GetFromRight (20),
                                                   COLOR_RED.WithContrast (0.8F)),
                                  CClippingLed);

        pGraphics->AttachControl (new ITextControl (b.GetReducedFromTop (130).GetFromTop (14).GetReducedFromRight (45).GetFromRight (37),
                                                    "Clipping",
                                                    IText (14,
                                                           rar::graphics::layout::RAR_COLOR_WORNOUT_WHITE,
                                                           "IBMPlexSans")));

        // Group Bits and sampleRate
        pGraphics->AttachControl (new rar::graphics::controls::RarGroupControl (b.GetReducedFromTop (60).GetFromTop (90).GetCentredInside (400.0F, 120.0F), "Main Controls", 150.0F));

        // Knobs
        pGraphics->AttachControl (new rar::graphics::controls::RarVectorKnob (b.GetReducedFromTop (60).GetFromTop (100).GetFromLeft (PLUG_WIDTH / 2).GetReducedFromLeft (PLUG_WIDTH / 6).GetCentredInside (100),
                                                                              KBits,
                                                                              "",
                                                                              redKnob,
                                                                              true))
            ->SetActionFunction (ShowBubbleHorizontalActionFunc);

        pGraphics->AttachControl (new rar::graphics::controls::RarVectorKnob (b.GetReducedFromTop (60).GetFromTop (100).GetFromRight (PLUG_WIDTH / 2).GetReducedFromRight (PLUG_WIDTH / 6).GetCentredInside (100),
                                                                              KSampleRate,
                                                                              "",
                                                                              blueKnob,
                                                                              true))
            ->SetActionFunction (ShowBubbleHorizontalActionFunc);

        pGraphics->AttachControl (new rar::graphics::controls::RarVectorKnob (b.GetReducedFromTop (170).GetFromTop (100).GetFromLeft (PLUG_WIDTH / 3).GetCentredInside (100),
                                                                              KInputGain,
                                                                              "",
                                                                              defaultKnob,
                                                                              true))
            ->SetActionFunction (ShowBubbleHorizontalActionFunc);

        pGraphics->AttachControl (new rar::graphics::controls::RarVectorKnob (b.GetReducedFromTop (170).GetFromTop (100).GetReducedFromLeft (PLUG_WIDTH / 3).GetReducedFromRight (PLUG_WIDTH / 3).GetCentredInside (100),
                                                                              KMix,
                                                                              "",
                                                                              defaultKnob,
                                                                              true))
            ->SetActionFunction (ShowBubbleHorizontalActionFunc);

        pGraphics->AttachControl (new rar::graphics::controls::RarVectorKnob (b.GetReducedFromTop (170).GetFromTop (100).GetFromRight (PLUG_WIDTH / 3).GetCentredInside (100),
                                                                              KOutputGain,
                                                                              "",
                                                                              defaultKnob,
                                                                              true))
            ->SetActionFunction (ShowBubbleHorizontalActionFunc);

        // Filter Buttons
        pGraphics->AttachControl (new rar::graphics::controls::RarToggleControl (b.GetReducedFromTop (240).GetFromTop (100).GetFromLeft (PLUG_WIDTH / 2).GetReducedFromLeft (PLUG_WIDTH / 6).GetCentredInside (36.0F, 28.0F),
                                                                                 KLpActive,
                                                                                 "",
                                                                                 buttonStyle,
                                                                                 "",
                                                                                 ""),
                                  kNoTag,
                                  "");

        pGraphics->AttachControl (new rar::graphics::controls::RarTextToggleControl (b.GetReducedFromTop (237).GetFromTop (100).GetFromLeft (PLUG_WIDTH / 2).GetReducedFromLeft (PLUG_WIDTH / 6).GetCentredInside (32.0F, 32.0F),
                                                                                     KLpActive,
                                                                                     ICON_FAD_FILTER_LOWPASS,
                                                                                     ICON_FAD_FILTER_LOWPASS,
                                                                                     IText (20.0F, rar::graphics::layout::RAR_COLOR_GREEN, "fontaudio"),
                                                                                     IText (20.0F, rar::graphics::layout::RAR_COLOR_RED, "fontaudio")));

        // HighPass Button
        pGraphics->AttachControl (new IVToggleControl (b.GetReducedFromTop (237).GetFromTop (100).GetFromRight (PLUG_WIDTH / 2).GetReducedFromRight (PLUG_WIDTH / 6).GetCentredInside (36.0F, 28.0F),
                                                       KHpActive,
                                                       "",
                                                       buttonStyleIV,
                                                       "",
                                                       ""),
                                  kNoTag,
                                  "");

        pGraphics->AttachControl (new rar::graphics::controls::RarTextToggleControl (b.GetReducedFromTop (237).GetFromTop (100).GetFromRight (PLUG_WIDTH / 2).GetReducedFromRight (PLUG_WIDTH / 6).GetCentredInside (32.0F, 32.0F),
                                                                                     KHpActive,
                                                                                     ICON_FAD_FILTER_HIGHPASS,
                                                                                     ICON_FAD_FILTER_HIGHPASS,
                                                                                     IText (20.0F, rar::graphics::layout::RAR_COLOR_GREEN, "fontaudio"),
                                                                                     IText (20.0F, rar::graphics::layout::RAR_COLOR_RED, "fontaudio")));

        // Clipping Button
        pGraphics->AttachControl (new rar::graphics::controls::RarToggleButton (b.GetReducedFromTop (237).GetFromTop (100).GetCentredInside (100.0F, 28.0F),
                                                                                KClippingEnabled),
                                  CClippingButton,
                                  "");

#pragma endregion Middle

#pragma region BottomBar
        pGraphics->AttachControl (new rar::graphics::controls::RarPanel (barBottom, IColor (255, 17, 17, 17), false));
        pGraphics->AttachControl (new ITextControl (barBottom.GetFromBottom (10.0F),
                                                    "TEST THINGY",
                                                    IText (DEFAULT_TEXT_SIZE, rar::graphics::layout::RAR_COLOR_WORNOUT_WHITE, "IBMPlexSans", EAlign::Near)),
                                  CInfo,
                                  "UI");

#pragma endregion BottomBar
    }; // layout
#endif
}

#if IPLUG_DSP
void PLUG_CLASS_NAME::ProcessBlock (sample** inputs, sample** outputs, int nFrames)
{
    /* Parameters linking */
    const auto bits = GetParam (KBits)->Value();
    const auto targetSampleRate = GetParam (KSampleRate)->Value();
    const auto inputGain = GetParam (KInputGain)->Value() / 100.0; // percentage
    const auto mix = GetParam (KMix)->Value() / 100.0; // percentage
    const auto outputGain = GetParam (KOutputGain)->Value() / 100.0; // percentage
    const auto clipping = GetParam (KClippingEnabled)->Value();
    const auto active = GetParam (KIsActive)->Bool();
    const auto lpactive = GetParam (KLpActive)->Bool();
    const auto hpactive = GetParam (KHpActive)->Bool();

    double tmpSignal;

    clippingSignal.store (false);
    bitCrusher->setBits (bits);
    downSampler->configure (targetSampleRate, GetSampleRate(), GetSamplePos());

    over_sampler_.ProcessBlock (inputs, outputs, nFrames, 2, 2, [&] (sample** inputs, sample** outputs, int nFrames)
                                {
                                    for (auto s = 0; s < nFrames; ++s)
                                    {
                                        for (auto c = 0; c < NOutChansConnected(); ++c)
                                        {
                                            if (active)
                                            {
                                                const auto drySignal = inputs[c][s] * inputGain;

                                                // Clipping
                                                if (clipping && drySignal > 1.0)
                                                {
                                                    tmpSignal = 1.0;
                                                    clippingSignal.store (true);
                                                }
                                                else if (clipping && drySignal < -1.0)
                                                {
                                                    tmpSignal = -1.0;
                                                    clippingSignal.store (true);
                                                }
                                                else
                                                    tmpSignal = drySignal;

                                                tmpSignal = bitCrusher->crush (tmpSignal, 0.0);
                                                tmpSignal = downSampler->resample (tmpSignal, c);

                                                // post filter
                                                if (lpactive)
                                                {
                                                    lowPassSvf[c]->setCutoffFreq (rar::rLimit (40.0, 20000.0, targetSampleRate));
                                                    tmpSignal = lowPassSvf[c]->processAudioSample (tmpSignal, c);
                                                }

                                                if (hpactive)
                                                    tmpSignal = highPassSvf[c]->processAudioSample (tmpSignal, c);

                                                // Mix
                                                tmpSignal = tmpSignal * mix + drySignal * (1.0 - mix);

                                                outputs[c][s] = tmpSignal * outputGain;
                                            }
                                            else
                                                outputs[c][s] = inputs[c][s];
                                        }
                                        downSampler->nextSample();
                                    }
                                });
}

void PLUG_CLASS_NAME::OnIdle()
{
    // get "idle" call on main thread
    // meters and such

    if (clippingSignal.load())
        SendControlValueFromDelegate (CClippingLed, 1.0);
    else
        SendControlValueFromDelegate (CClippingLed, 0.0);

    if (GetUI())
        dynamic_cast<ITextControl*> (GetUI()->GetControlWithTag (CInfo))->SetStrFmt (69, "Samplerate: %6.0f | Framesize: %d | Channels: %d/%d", GetSampleRate(), GetBlockSize(), NInChansConnected(), NOutChansConnected());
}

void PLUG_CLASS_NAME::OnParamChangeUI (int paramIdx, EParamSource source)
{
    if (auto graphics = GetUI())
    {
        /*
        const auto b = GetUI()->GetBounds();
        const auto bottomBar = b.GetFromBottom (20.0F);
        GetUI()->RemoveControlWithTag (CInfoText);
        GetUI()->AttachControl (new rar::graphics::display::RarInfoText (bottomBar,
                                                                         paramIdx,
                                                                         IText (14,
                                                                                rar::graphics::layout::RAR_COLOR_WORNOUT_WHITE,
                                                                                "IBMPlexSans",
                                                                                EAlign::Center)),
                                CInfoText);
        */

        if (GetParam (KIsActive)->Value() == 0)
        {
            GetUI()->DisableControl (KBits, true);
            GetUI()->DisableControl (KSampleRate, true);
            GetUI()->DisableControl (KMix, true);
            GetUI()->DisableControl (KInputGain, true);
            GetUI()->DisableControl (KOutputGain, true);
            GetUI()->DisableControl (KLpActive, true);
            GetUI()->DisableControl (KHpActive, true);
            GetUI()->DisableControl (KClippingEnabled, true);
        }
        else if (GetParam (KIsActive)->Value() == 1)
        {
            GetUI()->DisableControl (KBits, false);
            GetUI()->DisableControl (KSampleRate, false);
            GetUI()->DisableControl (KMix, false);
            GetUI()->DisableControl (KInputGain, false);
            GetUI()->DisableControl (KOutputGain, false);
            GetUI()->DisableControl (KLpActive, false);
            GetUI()->DisableControl (KHpActive, false);
            GetUI()->DisableControl (KClippingEnabled, false);
        }

    }
}

void PLUG_CLASS_NAME::OnReset()
{
    // do something prior to playback
    // clear buffers, update internal DSP with latest sampleRate

    /* Initialize filters and construct them */
    for (auto ch = 0; ch < 2; ++ch)
    {
        highPassSvf[ch] = std::make_unique<rar::dsp::svf::ZSvf<double>>(); // 200 Hz
        lowPassSvf[ch] = std::make_unique<rar::dsp::svf::ZSvf<double>>(); // BrickWall Filter

        noise[ch] = std::make_unique<rar::utils::noise::AmpNoise>();
        noise[ch]->setGain (0.0012);

        highPassSvf[ch]->setFilter (rar::dsp::svf::ZSvf<double>::SvfType::SvfHighpass, 200, 0.71, 0.0); // 200 Hz
        highPassSvf[ch]->setSmoothing (1.0);
        highPassSvf[ch]->setSampleRate (GetSampleRate());

        lowPassSvf[ch]->setFilter (rar::dsp::svf::ZSvf<double>::SvfType::SvfLowpass, 80, 0.71, 0.0); // BrickWall Filter WIP
        lowPassSvf[ch]->setSmoothing (1.0);
        lowPassSvf[ch]->setSampleRate (GetSampleRate());
    }

    /* Initialize Effects */
    bitCrusher = std::make_unique<rar::dsp::BitCrusher<double>>();
    downSampler = std::make_unique<rar::dsp::DownSampler<double>>();

    over_sampler_.Reset (GetBlockSize());
}

#endif
