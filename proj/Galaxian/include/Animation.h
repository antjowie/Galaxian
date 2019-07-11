#pragma once

// This class returns the correct uv values needed for animation management
class Animation
{
public:

	Animation(const unsigned int animationCount, const float animationDuration);

	void update(const float elapsedTime);

	void getCurrentAnimation(float &minU, float &minV, float &maxU, float &maxV) const;

	void setAnimation(const int currentAnim);

private:

	const unsigned int m_animCount;
	unsigned int m_currentAnim;
	
	float m_elapsedTime;
	const float m_animDuration;
};