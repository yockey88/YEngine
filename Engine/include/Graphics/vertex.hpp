#ifndef VERTEX_HPP
#define VERTEX_HPP

#include "log.hpp"

#include "glad/glad.h"

#include <iostream>
#include <memory>
#include <vector>
#include <type_traits>
#include <stdint.h>

#define MACHY_CREATE_VERTEX_BUFFER(name , type) std::unique_ptr<machy::graphics::VertexBuffer<type>> name = std::make_unique<machy::graphics::VertexBuffer<type>>()

namespace machy {
namespace graphics {

    class RawVertexBuffer {
        protected:
            uint32_t vbo = 0;
            uint32_t vertCount = 0;
            uint32_t stride = 0;
            uint32_t GLType = 0;
            uint32_t size = 0;
            std::vector<uint32_t> layout;
            void* data = nullptr;

            bool uploaded = false;
        public:
            static const uint32_t GLTypeByte;
            static const uint32_t GLTypeUByte;
            static const uint32_t GLTypeShort;
            static const uint32_t GLTypeUShort;
            static const uint32_t GLTypeInt;
            static const uint32_t GLTypeUInt;
            static const uint32_t GLTypeFloat;
            static const uint32_t GLTypeDouble;

            RawVertexBuffer();
            virtual ~RawVertexBuffer();

            void setLayout(const std::vector<uint32_t>& layout);

            virtual uint32_t getTypeSize() const = 0;

            virtual void upload(bool dynamic);

            void bind();
            void unbind();

            inline uint32_t getID() const             { return vbo; }
            inline uint32_t getVertCount() const      { return vertCount; }
            inline uint32_t getStride() const         { return stride; }
            inline uint32_t getSize() const           { return size; }
            inline uint32_t getGLType() const         { return GLType; }
            inline bool isUploaded() const            { return uploaded; }
            inline std::vector<uint32_t>& getLayout() { return layout; }
    };

    template <typename T>
    class VertexBuffer : public RawVertexBuffer {
        static_assert(std::is_same<T , char>()  || std::is_same<T , unsigned char>()  ||
                      std::is_same<T , short>() || std::is_same<T , unsigned short>() ||
                      std::is_same<T , int>()   || std::is_same<T , unsigned int>()   ||
                      std::is_same<T , float>() || std::is_same<T , double>() , 
                      "Vertex data type not suppored");

        uint32_t valueCount;
        std::vector<T> dataVec;
        public:
            VertexBuffer() : valueCount(0) {
                if constexpr (std::is_same<T , char>())           { GLType = RawVertexBuffer::GLTypeByte;   }
                if constexpr (std::is_same<T , unsigned char>())  { GLType = RawVertexBuffer::GLTypeUByte;  }
                if constexpr (std::is_same<T , short>())          { GLType = RawVertexBuffer::GLTypeShort;  }
                if constexpr (std::is_same<T , unsigned short>()) { GLType = RawVertexBuffer::GLTypeUShort; }
                if constexpr (std::is_same<T , int>())            { GLType = RawVertexBuffer::GLTypeInt;    }
                if constexpr (std::is_same<T , unsigned int>())   { GLType = RawVertexBuffer::GLTypeUInt;   }
                if constexpr (std::is_same<T , float>())          { GLType = RawVertexBuffer::GLTypeFloat;  }
                if constexpr (std::is_same<T , double>())         { GLType = RawVertexBuffer::GLTypeDouble; }
            }
            ~VertexBuffer() {}

            inline uint32_t getTypeSize() const override { return sizeof(T); }

            void pushVertex(const std::vector<T>& vert) {

                uint32_t receivedVals = (uint32_t)vert.size();

                MACHY_ASSERT(vert.size() > 0 , "Attempting to Push Empty Vertex");
                if (dataVec.size() == 0) {
                    valueCount = (uint32_t)vert.size();
                }

                MACHY_ASSERT(vert.size() == valueCount , "Attempting to Push an Invalid Vertex");
                if (vert.size() == valueCount) {
                    vertCount++;
                    dataVec.insert(dataVec.end() , vert.begin() , vert.end());
                }

                return;
            } 

            void upload(bool dynamic = false) override {
                
                stride *= sizeof(T);
                size = sizeof(T) * (uint32_t)dataVec.size();

                MACHY_TRACE("Uploading Vertex -> size = {} | stride = {}" , size , stride);
                MACHY_ASSERT(size > 0 , "Attempting to Upload Empty Vertex");

                data = &dataVec[0];
                RawVertexBuffer::upload(dynamic);

                return;
            }
    };

    class VertexArray {
        uint32_t vao , ebo;
        uint32_t vertCount , eltCount;
        uint32_t attribCount;
        bool valid;
        std::vector<std::unique_ptr<RawVertexBuffer>> vbos;
        public:
            VertexArray();
            ~VertexArray();

            inline uint32_t getVertCount() const { return vertCount; }
            inline uint32_t getEltCount() const  { return eltCount; }
            inline bool isValid() const          { return valid; }

            void pushBuffer(std::unique_ptr<RawVertexBuffer> vbo);
            void setElements(const std::vector<uint32_t>& elements);

            void upload();

            void bind();
            void unbind();

    };

} 
}

#endif