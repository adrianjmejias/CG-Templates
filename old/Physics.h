#pragma once
#include "types.h"



class GameObject;


class Collider {
public:
	virtual bool Intersection(Vec3 point) = 0;
};

class BoxCollider : public Collider /*: public Component*/{
public:
	Vec3 init;
	Vec3 end;
	// Inherited via Collider
	virtual bool Intersection(Vec3 point) override;

};


class Ray {
public:
	GameObject* goHit;
};

class Physics
{
public:
	Physics();
	~Physics();

	bool Raycast(Vec3 init, Vec3 director, float MaxLength, int MaxIter, Ray& ray, std::vector<Collider> col) {


		float delta = MaxLength/MaxIter;
		for (float length = 0; length < MaxLength; length += delta) {

			Vec3 actPoint = init + director * length;

			for (size_t ii = 0; ii < col.size(); ii++) {
				Collider& actCol = col[ii];

				if (actCol.Intersection(actPoint)) 
				{

					//ray.goHit = need to make collider a component so I can fcking know who it belongs to 
					return true;
				}
			}
		}

		return false;
	}
};

