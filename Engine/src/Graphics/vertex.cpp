#include "Graphics/vertex.hpp"
#include "Graphics/helper.hpp"

namespace machy {
namespace graphics {

    const uint32_t RawVertexBuffer::GLTypeByte   = GL_BYTE;
	const uint32_t RawVertexBuffer::GLTypeUByte  = GL_UNSIGNED_BYTE;
	const uint32_t RawVertexBuffer::GLTypeShort  = GL_SHORT;
	const uint32_t RawVertexBuffer::GLTypeUShort = GL_UNSIGNED_SHORT;
	const uint32_t RawVertexBuffer::GLTypeInt    = GL_INT;
	const uint32_t RawVertexBuffer::GLTypeUInt   = GL_UNSIGNED_INT;
	const uint32_t RawVertexBuffer::GLTypeFloat  = GL_FLOAT;
	const uint32_t RawVertexBuffer::GLTypeDouble = GL_DOUBLE;

    RawVertexBuffer::RawVertexBuffer() {
        glGenBuffers(1 , &vbo); MACHY_CHECK_GL_ERROR;
    }

    RawVertexBuffer::~RawVertexBuffer() {
        glDeleteBuffers(1 , &vbo); MACHY_CHECK_GL_ERROR;
    }

    void RawVertexBuffer::setLayout(const std::vector<uint32_t>& layout) {
        
        this->layout = layout;
        stride = 0;

        for (auto& count : layout)
            stride += count;

        return;
    }

    void RawVertexBuffer::upload(bool dynamic = false) {
        
        glBindBuffer(GL_ARRAY_BUFFER, vbo); MACHY_CHECK_GL_ERROR;
        glBufferData(GL_ARRAY_BUFFER , size , data , (dynamic) ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW); MACHY_CHECK_GL_ERROR;
        glBindBuffer(GL_ARRAY_BUFFER , 0);
        uploaded = true;

        return;
    }

    void RawVertexBuffer::bind() {

        glBindBuffer(GL_ARRAY_BUFFER , vbo);

        return;
    }

    void RawVertexBuffer::unbind() {

        glBindBuffer(GL_ARRAY_BUFFER , 0);

        return;
    }

    VertexArray::VertexArray() : vao(0) , ebo(0) , attribCount(0) , vertCount(0) , eltCount(0) , valid(false) { 
        glGenVertexArrays(1 , &vao); MACHY_CHECK_GL_ERROR;
    }

    VertexArray::~VertexArray() {
         vbos.clear();
        glDeleteVertexArrays(1 , &vao); MACHY_CHECK_GL_ERROR;
    }

    void VertexArray::pushBuffer(std::unique_ptr<RawVertexBuffer> vbo) {

        if (vbos.size() > 0)
            MACHY_ASSERT(vbos[0]->getVertCount() == vbo->getVertCount() , "Attempting to Push a Vertex Buffer with an Incorrect Number of Elements");

        MACHY_ASSERT(vbo->getLayout().size() > 0, "Vertex Buffer Layout is Undefined");
        if (vbo->getLayout().size() > 0) {
			vbos.push_back(std::move(vbo));
			vertCount = (uint32_t)vbos[0]->getVertCount();
		}

        return;
    }

    void VertexArray::setElements(const std::vector<uint32_t>& elements) {

        eltCount = (uint32_t)elements.size();
		glBindVertexArray(vao); MACHY_CHECK_GL_ERROR;
		glGenBuffers(1 , &ebo); MACHY_CHECK_GL_ERROR;
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER , ebo); MACHY_CHECK_GL_ERROR;
		glBufferData(GL_ELEMENT_ARRAY_BUFFER , elements.size() * sizeof(uint32_t) , &elements[0] , GL_STATIC_DRAW); MACHY_CHECK_GL_ERROR;
		glBindVertexArray(0); MACHY_CHECK_GL_ERROR;

        return;
    }

    void VertexArray::upload() {

        bind();
		uint32_t attributeCount = 0;
		for (auto& vbo : vbos) {
			if (!vbo->isUploaded())
				vbo->upload(false);

			vbo->bind();
			uint32_t offset = 0;
			for (uint32_t count : vbo->getLayout()) {
				glEnableVertexAttribArray(attributeCount); MACHY_CHECK_GL_ERROR;
				glVertexAttribPointer(attributeCount , count , static_cast<GLenum>(vbo->getGLType()) , GL_FALSE , vbo->getStride() , (void*)(intptr_t)offset); MACHY_CHECK_GL_ERROR;

				attributeCount++;
				offset += (count * vbo->getTypeSize());
			}
			vbo->unbind();
		}
		unbind();
		valid = true;

        return;
    }

    void VertexArray::bind() {

        glBindVertexArray(vao); MACHY_CHECK_GL_ERROR;

        return;
    }

    void VertexArray::unbind() {

        glBindVertexArray(0); MACHY_CHECK_GL_ERROR;

        return;
    }

}
}