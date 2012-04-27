#ifndef CLOTH
#define CLOTH

#include <d3d11.h>
#include <D3DX10math.h>

#include <vector>

#include "Api.h"
#include "Texture.h"
#include "Solver.h"

namespace pjs
{
	class Cloth
	{
	private:
		struct VertexType
		{
			Vec3 position;
			Vec3 normal;
			Vec2 tex;
		};
	public:
		Cloth();
		Cloth(int _width, int _height, float _spacing);
		~Cloth();

		void frame(Solver* _solver, float _timeStep, ID3D11DeviceContext*);

		bool initialize(ID3D11Device*,WCHAR*);
		void shutdown();
		void render(ID3D11DeviceContext*);
		int getIndexCount();
		ID3D11ShaderResourceView* getTexture();
		void getTranformationMatrix(Matrix &_mat);
	private:
		bool loadTexture(ID3D11Device*, WCHAR*);
		void releaseTexture();
		bool initializeBuffers(ID3D11Device*);
		void shutdownBuffers();
		void renderBuffers(ID3D11DeviceContext*);
		void updateBuffers(ID3D11DeviceContext*);
		void initializeBendSprings();
		void initializeShearSprings();
		void initializeStructSprings();
		void intializeParticles();

		void calculateSpringForce(Spring &_spring);	
		void accumulateForces(Solver* _solver);

		Texture* m_texture;

		Matrix m_transformationMatrix;

		ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
		int m_vertexCount, m_indexCount;

		int m_width, m_height;
		float m_spacing;

		std::vector<Particle> m_particles;
		std::vector<Spring> m_shearSprings;
		std::vector<Spring> m_bendSprings;
		std::vector<Spring> m_structSprings;
	};
}
#endif