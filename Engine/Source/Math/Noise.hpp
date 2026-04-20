#pragma once

class FastNoiseLite;

namespace Zenith {

    class Noise
    {
    public:
        Noise(int seed = 8);
        ~Noise();

        float GetFrequency() const { return m_Frequency; }
        void SetFrequency(float frequency);

        int GetFractalOctaves() const { return m_Octaves; }
        void SetFractalOctaves(int octaves);

        float GetFractalLacunarity() const { return m_Lacunarity; }
        void SetFractalLacunarity(float lacunarity);

        float GetFractalGain() const { return m_Gain; }
        void SetFractalGain(float gain);

        float Get(float x, float y);

        // Static API
        static void SetSeed(int seed);
        static float PerlinNoise(float x, float y);

    private:
        FastNoiseLite* m_FastNoise = nullptr;

        float m_Frequency  = 0.01f;
        int   m_Octaves    = 3;
        float m_Lacunarity = 2.0f;
        float m_Gain       = 0.5f;
    };

}