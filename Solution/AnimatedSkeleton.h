#pragma once
//#include <EnginePch.h>
#include "AnimatedMesh.h"
#include <string>
#include "AssimpHelper.h"

struct SkeletonBuffer
{
	glm::mat4 BoneTransforms[MAX_NUM_BONES];
	glm::mat4 Model;
};

class AnimatedSkeleton
{
public:
	AnimatedSkeleton() noexcept;
	~AnimatedSkeleton();

	void SetSkeletonTransform(const glm::mat4& transform) const noexcept;
	void SetAnimationTimeSeconds(float animationTimeSeconds) const noexcept;
	void SetAnimationTimeTicks(float animationTimeTicks) const noexcept;

	const SkeletonBuffer& GetSkeletonBuffer() const noexcept;
	glm::mat4& GetBoneTransform(unsigned int id) const noexcept;
	float GetAnimationTimeSeconds() const noexcept;
	float GetAnimationTimeTicks() const noexcept;

	void UpdateBoneTransforms(float dtS, const AnimatedMesh* mesh) const;

private:
	void ReadNodeHeirarchy(const void* pNode,
		const void* pScene,
		const glm::mat4& parentTransform,
		const glm::mat4& globalInverseTransform,
		std::map<std::string, unsigned int>& boneMap,
		std::vector<glm::mat4>& boneOffsets) const;

	const void* FindNodeAnim(const void* pAnimation, const std::string& NodeName) const;

private:
	mutable SkeletonBuffer m_SkeletonBuffer;
	mutable float m_AnimationTimeSeconds;
	mutable float m_AnimationTimeTicks;
};

inline const SkeletonBuffer& AnimatedSkeleton::GetSkeletonBuffer() const noexcept
{
	return m_SkeletonBuffer;
}

inline glm::mat4 & AnimatedSkeleton::GetBoneTransform(unsigned int id) const noexcept
{
	return this->m_SkeletonBuffer.BoneTransforms[id];
}

inline float AnimatedSkeleton::GetAnimationTimeSeconds() const noexcept
{
	return m_AnimationTimeSeconds;
}

inline float AnimatedSkeleton::GetAnimationTimeTicks() const noexcept
{
	return m_AnimationTimeTicks;
}
