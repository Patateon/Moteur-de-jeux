
#include <common/mesh.hpp>

Mesh::Mesh(
    std::vector<glm::vec3> vertices,
    std::vector<unsigned short> indices
) 
{
    m_vertexPositions = vertices;
    m_triangleIndices = indices;
}

Mesh::~Mesh() {
    clear();
}

void Mesh::recomputeNormals() {
	m_vertexNormals.clear ();
    m_vertexNormals.resize (m_vertexPositions.size (), glm::vec3 (0.0, 0.0, 0.0));

    for(unsigned int tIt = 0; tIt < m_triangleIndices.size(); tIt+=3) {
        glm::uvec3 t = glm::uvec3(m_triangleIndices[tIt], m_triangleIndices[tIt+1], m_triangleIndices[tIt+2]);
        glm::vec3 n_t = glm::cross( m_vertexPositions[t[1]] - m_vertexPositions[t[0]] , m_vertexPositions[t[2]] - m_vertexPositions[t[0]] );
        m_vertexNormals[ t[0] ] += n_t;
        m_vertexNormals[ t[1] ] += n_t;
        m_vertexNormals[ t[2] ] += n_t;
    }
    for(unsigned int nIt = 0; nIt < m_vertexNormals.size(); ++nIt) {
        glm::normalize( m_vertexNormals[nIt] );
    }
}

void Mesh::recomputeTextureCoordinates() {
    m_vertexTexCoords.clear ();
    m_vertexTexCoords.resize (m_vertexPositions.size (), glm::vec2 (0.0, 0.0));

    float xMin = FLT_MAX , xMax = FLT_MIN;
    float yMin = FLT_MAX , yMax = FLT_MIN;
    for ( glm::vec3 & p : m_vertexPositions) {
        xMin = std::min( xMin , p[0] );
        xMax = std::max( xMax , p[0] );
        yMin = std::min( yMin , p[2] );
        yMax = std::max( yMax , p[2] );
    }
    for( unsigned int pIt = 0 ; pIt < m_vertexTexCoords.size() ; ++pIt ) {
        m_vertexTexCoords[ pIt ] = glm::vec2( (m_vertexPositions[pIt][0] - xMin)/(xMax-xMin) , (m_vertexPositions[pIt][2] - yMin)/(yMax-yMin) );
    }
}

glm::vec3 Mesh::getBarycentre() {
    glm::vec3 bc = glm::vec3();

    if (m_vertexPositions.size() == 0) {
        return bc;
    }

    for(int i = 0; i < m_vertexPositions.size(); i++) {
        bc += m_vertexPositions[i];
    }
    bc /= m_vertexPositions.size();

    return bc;
}

void Mesh::init() {
    // Send all buffers
    glGenBuffers(1, &m_posVbo);
    glGenBuffers(1, &m_normalVbo);
    glGenBuffers(1, &m_texCoordVbo);
    glGenBuffers(1, &m_ebo);
    glGenVertexArrays(1, &m_vao);

    glBindVertexArray(m_vao);

    // Vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, m_posVbo);
    glBufferData(GL_ARRAY_BUFFER, m_vertexPositions.size() * sizeof(glm::vec3),
                m_vertexPositions.data(), GL_STATIC_DRAW);   
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0); 
    
    // Normals
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, m_normalVbo);
    glBufferData(GL_ARRAY_BUFFER, m_vertexNormals.size() * sizeof(glm::vec3),
                m_vertexNormals.data(), GL_STATIC_DRAW);   
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0); 

    // Texture Coordinates
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, m_texCoordVbo);
    glBufferData(GL_ARRAY_BUFFER, m_vertexTexCoords.size() * sizeof(glm::vec2),
                m_vertexTexCoords.data(), GL_STATIC_DRAW);   
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0); 

    // Triangle indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
                m_triangleIndices.size() * sizeof(unsigned short), 
                m_triangleIndices.data() , GL_STATIC_DRAW);
    glBindVertexArray(0);
}

void Mesh::render() {
    glBindVertexArray(m_vao);
    glDrawElements(
        GL_TRIANGLES, 
        m_triangleIndices.size(), 
        GL_UNSIGNED_SHORT, 
        0
    );
    glBindVertexArray(0);
}

void Mesh::clear() {
	m_vertexPositions.clear ();
	m_vertexNormals.clear ();
	m_vertexTexCoords.clear ();
	m_triangleIndices.clear ();
	if (m_vao) {
		glDeleteVertexArrays (1, &m_vao);
		m_vao = 0;
	}
	if(m_posVbo) {
		glDeleteBuffers (1, &m_posVbo);
		m_posVbo = 0;
	}
	if (m_normalVbo) {
		glDeleteBuffers (1, &m_normalVbo);
		m_normalVbo = 0;
	}
	if (m_texCoordVbo) {
		glDeleteBuffers (1, &m_texCoordVbo);
		m_texCoordVbo = 0;
	}
	if (m_ebo) {
		glDeleteBuffers (1, &m_ebo);
		m_ebo = 0;
	}
}
