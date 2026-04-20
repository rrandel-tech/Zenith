#include "znpch.hpp"
#include "Noise.hpp"

#include <FastNoiseLite.h>

namespace Zenith {

	static FastNoiseLite s_FastNoise;

	Noise::Noise(int seed)
	{
		m_FastNoise = new FastNoiseLite(seed);
		m_FastNoise->SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
		m_FastNoise->SetFrequency(m_Frequency);
		m_FastNoise->SetFractalOctaves(m_Octaves);
		m_FastNoise->SetFractalLacunarity(m_Lacunarity);
		m_FastNoise->SetFractalGain(m_Gain);
	}

	Noise::~Noise()
	{
		delete m_FastNoise;
	}

	void Noise::SetFrequency(float frequency)
	{
		m_Frequency = frequency;
		m_FastNoise->SetFrequency(frequency);
	}

	void Noise::SetFractalOctaves(int octaves)
	{
		m_Octaves = octaves;
		m_FastNoise->SetFractalOctaves(octaves);
	}

	void Noise::SetFractalLacunarity(float lacunarity)
	{
		m_Lacunarity = lacunarity;
		m_FastNoise->SetFractalLacunarity(lacunarity);
	}

	void Noise::SetFractalGain(float gain)
	{
		m_Gain = gain;
		m_FastNoise->SetFractalGain(gain);
	}

	float Noise::Get(float x, float y)
	{
		return m_FastNoise->GetNoise(x, y);
	}

	void Noise::SetSeed(int seed)
	{
		s_FastNoise.SetSeed(seed);
	}

	float Noise::PerlinNoise(float x, float y)
	{
		s_FastNoise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
		return s_FastNoise.GetNoise(x, y); // [-1, 1]
	}

}