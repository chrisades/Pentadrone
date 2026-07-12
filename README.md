# Pentadrone

A five-voice wavetable drone synthesizer built as a JUCE audio plugin. Each of the five "drones" is triggered by a dedicated MIDI note and layers five morphable wavetable oscillators through a stereo state-variable filter, with five LFOs available to modulate almost any parameter via a per-slot modulation matrix.

## Overview

Pentadrone is polyphonic in a specific sense: it isn't played like a keyboard, it's played like a drone box. Five fixed MIDI notes each control one "drone" voice slot. Triggering a note starts (or, in Hold mode, latches) that slot's envelope; each active slot renders five detuned/morphed wavetable oscillators through its own stereo filter and outputs a continuous sustained texture. Five LFOs — shared in configuration but computed independently per drone — can be patched to modulate cutoff, resonance, drone level, transpose, or any individual oscillator's morph/pitch/pan/level, or any LFO's own morph/speed/depth.

## Features

- **5 drone voices**, each independently triggered/held via MIDI
- **5 wavetable oscillators per drone**, each with:
  - Continuous waveshape morphing (square → triangle → saw) across 127 morph steps, anti-aliased via a bank of band-limited wavetables selected by pitch
  - Independent frequency, level, and stereo pan
- **5 LFOs per drone** with their own waveshape morph, speed, and depth, each independently routable to any of 43 modulation destinations (a shared "modulation matrix" driven by an `AudioParameterChoice` per LFO)
- **Stereo State-Variable Filter** (TPT/"zero-delay feedback" topology) with independent left/right cutoff and resonance, plus filter-envelope depth (bipolar — can open *or* close the filter over the envelope)
- **Custom envelope generator** (attack / decay / sustain / release) driving both amplitude and filter cutoff
- **Note quantizer** — optional snap-to-12-TET for oscillator pitch
- **Global transpose** controls for pitch and wavetable morph, applied on top of each oscillator's own settings
- **Hold mode** — latches drones so they sustain after note-off, until re-triggered or Hold is disengaged
- **Output safety limiter** (`protectYourEars`) — clamps/silences NaN, Inf, and out-of-range samples before they reach the audio hardware

## Signal Flow

```
MIDI Note (60/62/64/65/67)
      │
      ▼
 Drone Envelope (attack/decay/sustain/release)
      │
      ▼
 5× WavetableOsc  ──►  Pan (per-oscillator, equal-power)  ──►  Sum (L/R)
      │
      ▼
 Stereo SVF Filter (cutoff/reso, modulated by envelope + LFO)
      │
      ▼
 Drone Level (envelope-scaled, oscillator-count normalized)
      │
      ▼
 Sum across all 5 drones  ──►  Master Level  ──►  Output

 5× LFO (per drone) ──► Modulation Matrix (Parameters.h enum) ──► any of the above
```

## MIDI Mapping

| MIDI Note | Note Name | Drone Slot |
|-----------|-----------|------------|
| 60 | C4 | Drone 1 |
| 62 | D4 | Drone 2 |
| 64 | E4 | Drone 3 |
| 65 | F4 | Drone 4 |
| 67 | G4 | Drone 5 |

All other notes are ignored. Note-off releases the corresponding drone's envelope (unless **Hold** is enabled, in which case the first note-off latches the drone and a second press-and-release retriggers/releases it).

## Modulation Matrix

Each LFO's destination is chosen from a shared list of 43 parameters:

- Global: `Cutoff L`, `Cutoff R`, `Reso L`, `Reso R`, `Drone Level`, `Morph Transpose`, `Freq Transpose`
- Per-oscillator (×5): `Morph`, `Pitch`, `Pan`, `Level`
- Per-LFO (×5): `Morph`, `Speed`, `Depth` (an LFO can modulate another LFO's own parameters)

At the parameter layer, an LFO destination is stored per-drone via `Drone::setModulator()`, and `Drone::getMod()` looks up the current LFO value for any modulatable parameter during rendering.

## Project Structure

```
PluginProcessor.h/.cpp   JUCE AudioProcessor: parameter layout (APVTS), MIDI→sample-accurate
                          event splitting, per-block parameter smoothing/mapping into Synth

PluginEditor.h/.cpp       Placeholder GUI

Synth.h/.cpp              Owns the 5 Drone voices, wavetable generation, MIDI note handling,
                          voice allocation, and final mixdown

Drone.h                   A single drone voice: 5 oscillators, 5 LFOs, envelope, stereo filter,
                          per-parameter modulation lookup

WavetableOsc.h/.cpp       Band-limited wavetable oscillator with Hermite interpolation

Wavetables.h              Generates morphable wavetable banks (square/tri/saw, and sine-inclusive
                          variant for LFOs) with harmonic count scaled to avoid aliasing

Envelope.h                Custom one-pole-based ADSR envelope generator

Filter.h                  Stereo TPT/zero-delay-feedback state-variable filter

OnePoleFilter.h            General-purpose one-pole smoothing filter (parameter smoothing)

Quantizer.h                Snaps a frequency to the nearest 12-tone-equal-temperament pitch

Parameters.h               Shared enum of modulation destinations + filter cutoff constants

Utils.h                    `protectYourEars` safety limiter + APVTS parameter-casting helper
```

## Parameters

All parameters are registered through `juce::AudioProcessorValueTreeState` in `PluginProcessor::createParameterLayout()`.

| Group | Parameters |
|---|---|
| Oscillators (×5) | Morph, Freq, Pan, Level |
| LFOs (×5) | Destination, Morph, Speed, Depth |
| Envelope | Attack, Decay, Sustain, Release |
| Drone | Level, Freq Transpose, Morph Transpose |
| Filter | Cutoff L/R, Resonance L/R, Env Depth |
| Global | Hold, Quantize |

## Building

This is a standard JUCE audio plugin project. To build it you'll need:

- [JUCE](https://juce.com/) framework
- A JUCE project file (`.jucer`) or CMake configuration targeting this source set
- A C++17-or-later toolchain

The plugin can be built as VST3/AU/standalone depending on how the project/build target is configured.

## Known Limitations

- **No preset persistence.** `getStateInformation` / `setStateInformation` are empty stubs — plugin state is not saved or restored by the host.

- **No drone persitence.** If one drone is selected, changes made to it will not persist, so any parameter change on one drone will change the state of all others. This can be fixed by adding functionality so that each drone object only changes it's parameters if the drone currently activated and a parameter is moved.

- **Startup Time.** Because of how many wavetables are generated for anti-aliasing, there is a startup time of about 45 Seconds. Though this is avoidable by reducing the amount of "mips" and adding an interpolation for them in `WavetableOsc.h/.cpp`.

- **Duophony.** Activating more than 2 drones at once causes audible distortion. Cause still unkown, though likely a computational overload.
