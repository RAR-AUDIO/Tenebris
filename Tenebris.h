// =============================================================================
//  Project     : Tenebris
//  Version     : 0.1.0
//
//  Category    : Tenebris-vst3
//  Filename    : Tenebris-vst3/Tenebris.h
//  Created by  : RAR-AUDIO, 02/2021
//  Author      : Roberto A. Ramirez Gonzalez
//  Description :
//
//
// -----------------------------------------------------------------------------
//  LICENSE
//  (c) 2021, RAR-AUDIO, All Rights Reserved
// -----------------------------------------------------------------------------
// =============================================================================

#ifndef TENEBRIS_H
#define TENEBRIS_H

//==================================================================================
/* iPlug Includes */
#include "IPlug_include_in_plug_hdr.h"

#include "Extras/Oversampler.h"
//==================================================================================
/* RarClasses Includes */
#include "DSP/RAR_Effects.hpp" //DownSampler and BitCrusher
#include "DSP/RAR_Filters.hpp" // Post HighPass, post anti aliasing

#include "Utils/RAR_Noise.hpp" // Noise
//==================================================================================

const int K_NUM_PRESETS = 1;

enum EParams
{
    KInputGain,
    KMix,
    KOutputGain,
    KBits,
    KSampleRate,
    KClippingEnabled,
    KIsActive,
    KLpActive,
    KHpActive,
    KNumParams
};

enum ECtrlTags
{
    CInfoText,
    CInfo,
    CClippingButton,
    CClippingLed,
    CSignalInLed,
    CNumCtrlTags
};

enum ETags
{
    kAboutButton,
    kTags
};

using namespace iplug;
using namespace igraphics;

class PLUG_CLASS_NAME final : public Plugin
{
public:
    PLUG_CLASS_NAME (const InstanceInfo& info);

#if IPLUG_DSP // http://bit.ly/2S64BDd
    void ProcessBlock (sample** inputs, sample** outputs, int nFrames) override;
    void OnReset() override;
    void OnIdle() override;
    void OnParamChangeUI (int paramIdx, EParamSource source) override;

#endif
private:
    /* Effects Pointer */
    std::unique_ptr<rar::dsp::DownSampler<double, 2>> downSampler;
    std::unique_ptr<rar::dsp::BitCrusher<double>> bitCrusher;
                         
    /* Filters */        
    std::unique_ptr<rar::dsp::svf::ZSvf<double>> lowPassSvf[2];
    std::unique_ptr<rar::dsp::svf::ZSvf<double>> highPassSvf[2];
                    
    /* Noise */     
    std::unique_ptr<rar::utils::noise::AmpNoise> noise[2];

    /* Clipping */
    std::atomic_bool clippingSignal;

    OverSampler<double> over_sampler_ { k4x, true, 2, 2 };
};

#endif // TENEBRIS_H