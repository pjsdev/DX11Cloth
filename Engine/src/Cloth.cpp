#include <sstream>
#include <iostream>

#include "Cloth.h"

using namespace pjs;

Cloth::Cloth()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_texture = 0;
}

Cloth::Cloth(int _width, int _height, float _spacing):m_width(_width),m_height(_height),m_spacing(_spacing)
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_vertexCount = m_indexCount = m_width*m_height;
	m_particles.resize(m_vertexCount);
}
Cloth::~Cloth(){} 

void Cloth::frame(Solver* _solver, float _timeStep, ID3D11DeviceContext* _context)
{
	accumulateForces(_solver);
	_solver->verlet(m_particles, _timeStep);
	updateBuffers(_context);
}

void Cloth::updateBuffers(ID3D11DeviceContext* _context)
{
	std::vector<VertexType> vertData;

	float s = 1.0f;
    float t = 1.0f;

    float sProportion = 1.0f/(float)m_width;
    float tProportion = 1.0f/(float)m_height;

	Vec3 normal;
	for(int i = 0; i < m_vertexCount; ++i)
	{
		VertexType tmp;
		tmp.position = m_particles[i].pos;

		tmp.tex = Vec2((s*sProportion), (t*tProportion));

        if((i+1)%m_width == 0)
        {
            s = 1.0f;
            t+=1.0f;
        }
        else
        {
            s+=1.0f;
        }

		Vec3 v2Out, v1Out, v1, v2;
		bool flip = false;
		if(i + m_width < m_width*m_height)
        {
            if((i+1)%m_width != 0)
            {
                v1 = m_particles[i+1].pos - m_particles[i].pos;
                v2 = m_particles[i+m_width].pos - m_particles[i].pos;
                
				
            }
            else
            {
                v1 = m_particles[i-1].pos - m_particles[i].pos;
                v2 = m_particles[i+m_width].pos - m_particles[i].pos;
				flip = true;
            }
        }
        else
        {
            if((i+1)%m_width != 0)
            {
                v1 = m_particles[i+1].pos - m_particles[i].pos;
                v2 = m_particles[i-m_width].pos - m_particles[i].pos;
				flip = true;
            }
            else
            {
                v1 = m_particles[i-1].pos - m_particles[i].pos;
                v2 = m_particles[i-m_width].pos - m_particles[i].pos;
            }
        }

		D3DXVec3Normalize(&v2Out, &v2);
		D3DXVec3Normalize(&v1Out, &v1);
		D3DXVec3Cross(&normal, &v1Out, &v2Out);

		if(flip)
			normal = -normal;

		tmp.normal = -normal;
		vertData.push_back(tmp);
	}

	D3D11_MAPPED_SUBRESOURCE mappedVertBuff;
	_context->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedVertBuff);
	memcpy(mappedVertBuff.pData, &vertData[0], (sizeof(VertexType) * m_vertexCount));
	_context->Unmap(m_vertexBuffer, 0);


}

bool Cloth::initialize(ID3D11Device* _device, WCHAR* _textureFilename)
{
	bool result;
	// Load the texture for this model.
	result = loadTexture(_device, _textureFilename);
	if(!result)
	{
		return false;
	}

	intializeParticles();
	initializeBendSprings();
	initializeShearSprings();
	initializeStructSprings();
	return initializeBuffers(_device);
}

ID3D11ShaderResourceView* Cloth::getTexture()
{
	return m_texture->getTexture();
}

void Cloth::shutdown()
{	
	// Release the model texture.
	releaseTexture();

	shutdownBuffers();
}

bool Cloth::loadTexture(ID3D11Device* device, WCHAR* filename)
{
	bool result;


	// Create the texture object.
	m_texture = new Texture;
	if(!m_texture)
	{
		return false;
	}

	// Initialize the texture object.
	result = m_texture->initialize(device, filename);
	if(!result)
	{
		return false;
	}

	return true;
}

void Cloth::releaseTexture()
{
	// Release the texture object.
	if(m_texture)
	{
		m_texture->shutdown();
		delete m_texture;
		m_texture = 0;
	}

	return;
}


void Cloth::render(ID3D11DeviceContext* _deviceContext)
{
	renderBuffers(_deviceContext);

	return;
}

int Cloth::getIndexCount()
{
	return m_indexCount;
}

bool Cloth::initializeBuffers(ID3D11Device* _device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
    D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	// Create the vertex array.
	vertices = new VertexType[m_vertexCount];
	if(!vertices)
	{
		return false;
	}
		
	m_indexCount = ((m_width-1)*6) * (m_height-1);
	// Create the index array.
	indices = new unsigned long[m_indexCount];
	if(!indices)
	{
		return false;
	}



	for(int i = 0; i < m_vertexCount; ++i)
	{
		vertices[i].position = m_particles[i].pos;
		vertices[i].normal = Vec3(0,0,0);
		vertices[i].tex = Vec2(0,0);
	}

	// these indices need to properly guide the vertex shader through triangle drawing of the cloth
	int idx = 0;

	for(int i = m_width; i < (m_vertexCount-1); ++i)
	{
		// if we hit an end point
		if( (i+1)%m_width == 0 )
		{
			continue;
		}

		indices[idx] = (unsigned long)i;
		indices[idx+1] = (unsigned long)i-m_width;
		indices[idx+2] = (unsigned long)i-(m_width-1);

		indices[idx+3] = (unsigned long)i;
		indices[idx+4] = (unsigned long)i-(m_width-1);
		indices[idx+5] = (unsigned long)i+1;

		idx += 6;
	}

	// Set up the description of the static vertex buffer.
    vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
    vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
    result = _device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if(FAILED(result))
	{
		std::ostringstream os;
		os << (LONG)result;
		MessageBox(NULL, (LPCWSTR)os.str().c_str(), (LPCWSTR)"Result", MB_SETFOREGROUND);
		return false;
	}

	// Set up the description of the static index buffer.
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
    indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = _device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete [] vertices;
	vertices = 0;

	delete [] indices;
	indices = 0;

	return true;
}

void Cloth::shutdownBuffers()
{
	// Release the index buffer.
	if(m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// Release the vertex buffer.
	if(m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	return;
}

void Cloth::renderBuffers(ID3D11DeviceContext* _deviceContext)
{
	unsigned int stride;
	unsigned int offset;

	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType); 
	offset = 0;
    
	// Set the vertex buffer to active in the input assembler so it can be rendered.
	_deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

    // Set the index buffer to active in the input assembler so it can be rendered.
	_deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

    // Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);

	return;
}

void Cloth::initializeBendSprings()
{
	int numOfBendSpringsHor = (m_width - 2) * m_height;
	int numOfBendSpringsVert = (m_height - 2) * m_width;

	for(int i = 0; i < numOfBendSpringsHor; ++i)
	{
		if(((i+1)%m_width) != 0)
		{
			Spring s;
			// Create horizontal bend springs
			s.p1 = &m_particles[i];
			s.p2 = &m_particles[i+2];

			Vec3 dist = s.p1->pos - s.p2->pos;

			s.restLength = D3DXVec3Length(&dist);
			s.damping = 100.0f;
			s.stiffness = 230.0f;
			m_bendSprings.push_back(s);
		}
	}

	for(int i = 0; i < numOfBendSpringsVert; ++i)
	{
		Spring s;
		// Create horizontal bend springs
		s.p1 = &m_particles[i];
		s.p2 = &m_particles[i+(m_width*2)];

		Vec3 dist = s.p1->pos - s.p2->pos;

		s.restLength = D3DXVec3Length(&dist);
		s.damping = 100.0f;
		s.stiffness = 230.0f;
		m_bendSprings.push_back(s);
	}
}
void Cloth::initializeShearSprings()
{
	for(int i = 0; i < (m_height - 1) * m_width; ++i)
	{
		if( ( (i+1)%m_width) != 0)
		{
			Spring s;

			// Create downward shear springs
			s.p1 = &m_particles[i];
			s.p2 = &m_particles[i+(m_width+1)];

			Vec3 dist = s.p1->pos - s.p2->pos;

			s.restLength = D3DXVec3Length(&dist);
			s.damping = 100.0f;
			s.stiffness = 230.0f;
			m_shearSprings.push_back(s);
		}
	}

	for(int i = m_width; i < m_width*m_height; ++i)
	{
		if(((i+1)%m_width) != 0)
		{
			// Create upward shear springs
			Spring s;
			s.p1 = &m_particles[i];
			s.p2 = &m_particles[i-(m_width-1)];

			Vec3 dist = s.p1->pos - s.p2->pos;

			s.restLength = D3DXVec3Length(&dist);
			s.damping = 100.0f;
			s.stiffness = 230.0f;
			m_shearSprings.push_back(s);
		}
	}
}
void Cloth::initializeStructSprings()
{
	for(unsigned int i = 0; i < m_particles.size(); i++)
	{
		if((i+1)%m_width != 0)
		{
			Spring s;
			s.p1 = &m_particles[i];
			s.p2 = &m_particles[i+1];
			Vec3 dist = s.p1->pos - s.p2->pos;
			s.restLength = D3DXVec3Length(&dist);
			s.damping = 100.0f;
			s.stiffness = 230.0f;
			m_structSprings.push_back(s);
		}
	}

	for(int i = 0; i < (m_height - 1) * m_width; i++)
	{
		if(i+m_width < (int)m_particles.size())
		{
			Spring s;
			s.p1 = &m_particles[i];
			s.p2 = &m_particles[i+m_width];
			Vec3 dist = s.p1->pos - s.p2->pos;
			s.restLength = D3DXVec3Length(&dist);
			s.damping = 100.0f;
			s.stiffness = 230.0f;
			m_structSprings.push_back(s);
		}
	}
}
void Cloth::intializeParticles()
{
	float y = (-((float)m_height)*0.5f)*m_spacing;
	float x = (-((float)m_width)*0.5f)*m_spacing;
	for(unsigned int i = 0; i < m_particles.size(); i++)
	{
		m_particles[i].pos = Vec3(x, y,0.0);
		m_particles[i].lastPos = Vec3(x, y,0.0);

		m_particles[i].velocity = Vec3(.0f, .0f, .0f);
		m_particles[i].acceleration = Vec3(.0f, .0f, .0f);
		m_particles[i].mass = 1.0;
		m_particles[i].isConstrained = false;
		m_particles[i].id = i;

		x += m_spacing;

		if( (i+1) % m_width == 0)
		{
			y -= m_spacing;
			x = (-((float)m_width)*0.5f)*m_spacing;
		}

		if((int)i < m_width)
		{
			m_particles[i].isConstrained = true;	
		}
	}
}

void Cloth::accumulateForces(Solver* _solver)
{
	_solver->accumulateForces(m_particles);

	_solver->calculateSpringForce(m_structSprings);

	_solver->calculateSpringForce(m_shearSprings);

	_solver->calculateSpringForce(m_bendSprings);
}