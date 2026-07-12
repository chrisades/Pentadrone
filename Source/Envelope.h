/*
  ==============================================================================

    Envelope.h
    Created: 3 Feb 2026 2:33:24pm
    Author:  user

  ==============================================================================
*/

#pragma once

const float SILENCE = 0.0001f;

class Envelope
{
public:
    float nextValue()
    {
        //1 pole filter: y[n] = (1-a) * x[n] + a * y[n-1]
        // same as:     level = (1-multiplier) * target + multiplier * level
        level = multiplier * (level - target) + target;

        if (level + target > 3.0f) {
            multiplier = decayMultiplier;
            target = sustainLevel;
        }

        return level;
    }

    void reset()
    {
        //DBG("arrived at reset");
        level = 0.0f;
        multiplier = 0.0f;
        target = 0.0f;
    }

    void release()
    {
        target = 0.0f;
        multiplier = releaseMultiplier;
    }

    inline bool isActive() const
    {
        return level > SILENCE;
    }

    inline bool isInAttack() const
    {
        return target >= 2.0f;
    }

    void attack()
    {
        level += SILENCE + SILENCE;  //makes sure isActive return true when attack stage is activated
        target = 2.0f;
        multiplier = attackMultiplier;

        //DBG("attack stage arrived");
        //DBG("level = " << level << "attack multiplier = " << multiplier);
    }

    float level;

    float attackMultiplier;
    float decayMultiplier;
    float sustainLevel;
    float releaseMultiplier;

private:
    float multiplier;
    float target;

};