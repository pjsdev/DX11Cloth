#include "Solver.h"

using namespace pjs;

Solver::Solver()
{}

Solver::~Solver()
{}

Solver::Solver(const Solver&)
{}

void Solver::verlet(std::vector<Particle> &_particles, float _timeStep)
{
	for(unsigned int i = 0; i < _particles.size(); i++)
    {
        if(!_particles[i].isConstrained)
        {
            Vec3 previousPos = _particles[i].pos;
            _particles[i].pos = _particles[i].pos + (_particles[i].pos - _particles[i].lastPos)
                    + ( _particles[i].acceleration * _timeStep) * _timeStep;
            _particles[i].lastPos = previousPos;

        }
    }
}

void Solver::calculateSpringForce(std::vector<Spring> &_springs)
{
	for(unsigned int i= 0; i < _springs.size(); i++)
	{
		float restLength = _springs[i].restLength;
		Vec3 dist = _springs[i].p1->pos - _springs[i].p2->pos;
		float currentLength = D3DXVec3Length(&dist);

		float extension = currentLength - restLength;

		Vec3 vel = _springs[i].p2->velocity - _springs[i].p1->velocity;

		Vec3 sNorm = _springs[i].p1->pos - _springs[i].p2->pos;
		Vec3 sNormResult;
		if(D3DXVec3Length(&sNorm) !=0 )
		{
			D3DXVec3Normalize(&sNormResult, &sNorm);
		}

		Vec3 stiffnessForce = Vec3(0,0,0);
		Vec3 springForce = Vec3(0,0,0);
		Vec3 dampingForce = Vec3(0,0,0);

		stiffnessForce = 1.0f * _springs[i].stiffness * extension * sNormResult;
		dampingForce = -1.0f * _springs[i].damping * vel;

		springForce = stiffnessForce + dampingForce;

		_springs[i].p1->acceleration += -springForce / _springs[i].p1->mass;
		_springs[i].p2->acceleration += springForce / _springs[i].p2->mass;
	}
}

bool Solver::initialize()
{
	m_forces.clear();
	return true;
}
void Solver::shutdown(){}

void Solver::addForce(const std::string &_name, const Vec3 &_force)
{
	m_forces[_name] = _force;
}

void Solver::removeForce(const std::string &_name)
{
	m_forces.erase(_name);
}

void Solver::editForce(const std::string &_name, const Vec3 &_modifier)
{
	m_forces[_name] += _modifier;
}

void Solver::accumulateForces(std::vector<Particle> &_particles)
{
	for(unsigned int i = 0; i < _particles.size(); i++)
    {
		_particles[i].acceleration = Vec3(0,0,0);

		for(std::map<std::string, Vec3>::iterator it = m_forces.begin(); it != m_forces.end(); it++)
		{
			_particles[i].acceleration += it->second / _particles[i].mass;
		}
    }
}