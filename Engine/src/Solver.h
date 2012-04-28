#ifndef SOLVER
#define SOLVER

#include <D3DX10math.h>

#include <string>
#include <map>
#include <vector>

#include "Api.h"

namespace pjs
{
	struct Particle
		{
			pjs::Vec3 pos, velocity, acceleration, lastPos;
			float mass;
			bool isConstrained;
			int id;
		};

	struct Spring
		{
			float restLength, stiffness, damping;
			Particle *p1, *p2;
		};
	class Solver
	{
	public:
		Solver();
		Solver(const Solver&);
		~Solver();

		bool initialize();
		void shutdown();

		void addForce(const std::string &_name, const pjs::Vec3 &_force);
		void removeForce(const std::string &_name);
		void editForce(const std::string &_name, const pjs::Vec3 &_modifier);
		void calculateSpringForce(std::vector<Spring>& _springs);
		void accumulateForces(std::vector<Particle> &_particles);

		void verlet(std::vector<Particle> &_particles, float _timeStep);

	private:
		std::map<std::string, pjs::Vec3> m_forces;
	};
}

#endif