#include "Animation.h"
#include <algorithm>

Animation::Animation(const unsigned int animationCount, const float animationDuration):
	m_animCount(animationCount),
	m_animDuration(animationDuration),
	m_elapsedTime(0),
	m_currentAnim(0)
{
}

void Animation::update(const float elapsedTime)
{
	m_elapsedTime += elapsedTime;
	
	if(m_elapsedTime > m_animCount * m_animDuration)
		m_elapsedTime = std::fmod(m_elapsedTime, m_animCount* m_animDuration);
	m_currentAnim = static_cast<int>(m_elapsedTime / m_animDuration);
}

void Animation::getCurrentAnimation(float & minU, float & minV, float & maxU, float & maxV) const
{
	minV = 0;
	maxV = 1;

	minU = (float)m_currentAnim / (float)m_animCount;
	maxU = (float)(m_currentAnim + 1) / (float)m_animCount;
}

void Animation::setAnimation(const int currentAnim)
{
	m_currentAnim = currentAnim < 0 ? 0 : (currentAnim > m_animCount ? m_animCount : currentAnim);
	m_elapsedTime = m_currentAnim * m_animDuration;
}
