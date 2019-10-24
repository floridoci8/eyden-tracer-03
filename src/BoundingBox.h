#pragma once

#include "types.h"
#include "ray.h"
// needed for infinity
#include <limits> 
struct Ray;

namespace {
	inline Vec3f Min3f(const Vec3f a, const Vec3f b)
	{
		return Vec3f(MIN(a.val[0], b.val[0]), MIN(a.val[1], b.val[1]), MIN(a.val[2], b.val[2]));
	}

	inline Vec3f Max3f(const Vec3f a, const Vec3f b)
	{
		return Vec3f(MAX(a.val[0], b.val[0]), MAX(a.val[1], b.val[1]), MAX(a.val[2], b.val[2]));
	}
}

/**
 * @brief Bounding Box class
 */
class CBoundingBox
{
public:
	CBoundingBox(void){
		// set m_min = infinity, m_max = -infinity
		clear();
	}
	~CBoundingBox(void)= default;
	
	/**
	 * @brief Resets the bounding box
	 * @details This function resets the member variables \b m_min and \b m_max
	 */
	void clear(void)
	{
		// --- PUT YOUR CODE HERE ---
		// here reset means m_min = infinity, m_max = -infinity
		// m_min = infinity
		m_min[0] = std::numeric_limits<float>::infinity();
		m_min[1] = std::numeric_limits<float>::infinity();
		m_min[2] = std::numeric_limits<float>::infinity();
		
		//m_max = -infinity
		m_max[0] = -std::numeric_limits<float>::infinity();
		m_max[1] = -std::numeric_limits<float>::infinity();
		m_max[2] = -std::numeric_limits<float>::infinity();		
	}
	
	/**
	 * @brief Extends the bounding box to contain point \b a
	 * @param a A point
	 */
	void extend(Vec3f a)
	{
		// --- PUT YOUR CODE HERE ---
		// use Min3f and Max3f from namespace
		m_min = Min3f(a, m_min);
		m_max = Max3f(a, m_max);
	}
	
	/**
	 * @brief Extends the bounding box to contain bounding box \b box
	 * @param box The second bounding box
	 */
	void extend(const CBoundingBox& box)
	{
		// --- PUT YOUR CODE HERE ---
		extend(box.m_min);
		extend(box.m_max);
	}
	
	/**
	 * @brief Checks if the current bounding box overlaps with the argument bounding box \b box
	 * @param box The secind bounding box to be checked with
	 */
	bool overlaps(const CBoundingBox& box)
	{
		// --- PUT YOUR CODE HERE ---
		bool o[3];
		o[0] = m_min[0] <= box.m_max[0] && box.m_min[0] <= m_max[0];
		o[1] = m_min[1] <= box.m_max[1] && box.m_min[1] <= m_max[1];
		o[2] = m_min[2] <= box.m_max[2] && box.m_min[2] <= m_max[2];
		return o[0] && o[1] && o[2];
	}
	
	/**
	 * @brief Clips the ray with the bounding box
	 * @param[in] ray The ray
	 * @param[in,out] t0 The distance from ray origin at which the ray enters the bounding box
	 * @param[in,out] t1 The distance from ray origin at which the ray leaves the bounding box
	 */
	void clip(const Ray& ray, float& t0, float& t1)
	{
		// --- PUT YOUR CODE HERE ---
		// partial reference: https://tavianator.com/fast-branchless-raybounding-box-intersections/
		/* We just find the minimum and maximum of tfar and tnear respectively
		   as shown in the lecture
		*/
		float tfarMin = -std::numeric_limits<float>::infinity();
		float tnearMax = std::numeric_limits<float>::infinity();
		float txnear, tynear, tznear, txfar, tyfar, tzfar;
		
		if(ray.dir[0] != 0){
			txnear = (m_min[0] - ray.org[0]) / ray.dir[0];
			txfar = (m_max[0] - ray.org[0]) / ray.dir[0];

			if(ray.dir[0] < 0)
				std::swap(txnear, txfar);
		}

		if(ray.dir[1] != 0){
			tynear = (m_min[1] - ray.org[1]) / ray.dir[1];
			tyfar = (m_max[1] - ray.org[1]) / ray.dir[1];

			if(ray.dir[1] < 0)
				std::swap(tynear, tyfar);
		}	

		tnearMax = std::max(txnear, tynear);
		tfarMin = std::min(txfar, tyfar);

		if(ray.dir[2] != 0){
			tznear = (m_min[2] - ray.org[2]) / ray.dir[2];
			tzfar = (m_max[2] - ray.org[2]) / ray.dir[2];

			if(ray.dir[2] < 0)
				std::swap(tznear, tzfar);
		}	

		tnearMax = std::max(tnearMax, tznear);
		tfarMin = std::min(tfarMin, tzfar);

		if(tnearMax <= tfarMin){
			t0 = tnearMax;
			t1 = tfarMin;
		}
	}
	
	
public:
	Vec3f m_min;	///< The minimal point defying the size of the bounding box
	Vec3f m_max;	///< The maximal point defying the size of the bounding box
};
