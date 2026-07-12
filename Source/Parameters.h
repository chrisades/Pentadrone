/*
  ==============================================================================

    Parameters.h
    Created: 19 Feb 2026 6:43:37pm
    Author:  user

  ==============================================================================
*/

#pragma once

enum Parameters { //should follow the parameters in the choices varible for the LFO destinations in PluginProcessor.cpp
    NONE, Cutoff_L, Cutoff_R, Reso_L, Reso_R, Drone_Level, Morph_Transpose, Freq_Transpose,
    Osc_1_Morph, Osc_1_Pitch, Osc_1_Pan, Osc_1_Level,
    Osc_2_Morph, Osc_2_Pitch, Osc_2_Pan, Osc_2_Level,
    Osc_3_Morph, Osc_3_Pitch, Osc_3_Pan, Osc_3_Level,
    Osc_4_Morph, Osc_4_Pitch, Osc_4_Pan, Osc_4_Level,
    Osc_5_Morph, Osc_5_Pitch, Osc_5_Pan, Osc_5_Level,
    LFO_1_Morph, LFO_1_Speed, LFO_1_Depth,
    LFO_2_Morph, LFO_2_Speed, LFO_2_Depth,
    LFO_3_Morph, LFO_3_Speed, LFO_3_Depth,
    LFO_4_Morph, LFO_4_Speed, LFO_4_Depth,
    LFO_5_Morph, LFO_5_Speed, LFO_5_Depth
};

const int PARAM_CHOICE_COUNT = 43; //update this if new parameters are implemented
const float FILTER_CUTOFF_MAX = 7000.0f;
const float FILTER_CUTOFF_MIN = 40.0f;