#ifndef VERTEX_BUFFER_OBJECT
#define VERTEX_BUFFER_OBJECT

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib,"glew32.lib")

#include <glew.h>
#include <vector>

typedef unsigned int UINT;
typedef unsigned char BYTE;

class VertexBufferObject
{
public:
	void CreateVBO(int t_Size = 0);
	void RleaseVBO();

	void* MapBufferToMemory(int iUsageHint);
	void* MapSubBufferToMemory(int iUsageHint, UINT uOffset, UINT uLength);
	void UnmapBuffer();

	void BindVBO(int t_iBufferType = GL_ARRAY_BUFFER);
	void LoadDataToGPU(int iUsageHint);

	void AddData(void* ptrData, UINT uDataSize);

	void* GetDataPointer();
	UINT GetBuffer();

	VertexBufferObject();

private:
	UINT iBuffer;
	int iSize;
	int iBufferType;
	std::vector<BYTE> data;

	bool bDataUploaded;

};

#endif