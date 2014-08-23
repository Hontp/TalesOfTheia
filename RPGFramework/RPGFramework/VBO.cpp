#include"VBO.h"

VertexBufferObject::VertexBufferObject()
{
	bDataUploaded = false;
}

/*------------------------------------------
Name: CreateVBO

Params: t_Size - initial size of buffer

Result: Create vertex buffer object
--------------------------------------------*/

void VertexBufferObject::CreateVBO(int t_Size)
{
	glGenBuffers(1, &iBuffer);
	data.reserve(t_Size);
	iSize = t_Size;
}

/*------------------------------------------
Name: ReleaseVBO

Params: none

Result: release VBO and free memory
--------------------------------------------*/

void VertexBufferObject::RleaseVBO()
{
	glDeleteBuffers(1, &iBuffer);
	bDataUploaded = false;
	data.clear();
}

/*------------------------------------------
Name: MapBufferToMemory

Params: iUsageHint - GL_READ_ONLY, GL_WRITE_ONLY

Result: Create vertex buffer object
--------------------------------------------*/

void* VertexBufferObject::MapBufferToMemory(int iUsageHint)
{
	if (!bDataUploaded)
		return NULL;

	void* ptrRes = glMapBuffer(iBufferType, iUsageHint);

	return ptrRes;
}

/*------------------------------------------
Name: MapBufferToMemory

Params: iUsageHint - GL_READ_ONLY,GL_WRITE_ONLY
		uOffset - data offset(from where data should be maped)
		uLength - length of data

Result: maps buffer to memory
--------------------------------------------*/

void* VertexBufferObject::MapSubBufferToMemory(int iUsageHint, UINT uOffset, UINT uLength)
{
	if (!bDataUploaded)
		return NULL;

	void* ptrRes = glMapBufferRange(iBufferType, uOffset,uLength ,iUsageHint);

	return ptrRes;
}

/*------------------------------------------
Name: UnMapBuffer

Params: none

Result: Unmap mapped buffers
--------------------------------------------*/

void VertexBufferObject::UnmapBuffer()
{
	glUnmapBuffer(iBufferType);
}

/*------------------------------------------
Name: BindVBO

Params: t_iBufferType = buffer type

Result: Create vertex buffer object
--------------------------------------------*/

void VertexBufferObject::BindVBO(int t_iBufferType)
{
	iBufferType = t_iBufferType;

	glBindBuffer(iBufferType, iBuffer);
}

/*------------------------------------------
Name: LoadDataToGPU

Params: iUsageHint - a defined type (GL_STATIC_DRAW, 
					 GL_DYNAMIC_DRAW)

Result: Send data to GPU for processing
--------------------------------------------*/

void VertexBufferObject::LoadDataToGPU(int iUsageHint)
{
	glBufferData(iBufferType, data.size(), &data[0], iUsageHint);
	bDataUploaded = true;
	data.clear();
}

/*------------------------------------------
Name: AddData

Params: ptrData - pointer to arbitrary data
		uDataSize - data size in bytes

Result: Add arbitrary data to VBO
--------------------------------------------*/

void VertexBufferObject::AddData(void* ptrData, UINT uDataSize)
{
	data.insert(data.end(), (BYTE*)ptrData, (BYTE*)ptrData + uDataSize);
}

/*------------------------------------------
Name: GetDataPointer

Params: none

Result: Returns data pointer
--------------------------------------------*/

void* VertexBufferObject::GetDataPointer()
{
	if (bDataUploaded)
		return NULL;

	return (void*)data[0];
}

/*------------------------------------------
Name: GetBuffer

Params: none

Result: returns Vertex buffer object ID
--------------------------------------------*/

UINT VertexBufferObject::GetBuffer()
{
	return iBuffer;
}