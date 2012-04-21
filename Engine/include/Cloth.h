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
			vec3 position;
			vec3 normal;
			vec2 tex;
		};
	public:
		Cloth();
		Cloth(int _width, int _height, float _spacing);
		~Cloth();

		void Frame(Solver* _solver, float _timeStep, ID3D11DeviceContext*);

		bool Initialize(ID3D11Device*,WCHAR*);
		void Shutdown();
		void Render(ID3D11DeviceContext*);
		int GetIndexCount();
		ID3D11ShaderResourceView* GetTexture();
	private:
		bool LoadTexture(ID3D11Device*, WCHAR*);
		void ReleaseTexture();
		bool InitializeBuffers(ID3D11Device*);
		void ShutdownBuffers();
		void RenderBuffers(ID3D11DeviceContext*);
		void UpdateBuffers(ID3D11DeviceContext*);
		void initializeBendSprings();
		void initializeShearSprings();
		void initializeStructSprings();
		void intializeParticles();

		void calculateSpringForce(Spring &_spring);
		void accumulateForces(Solver* _solver);

		TextureClass* m_Texture;

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