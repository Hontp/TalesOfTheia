TalesOfTheia
============

Games Development

Changes 
=========


the following changes are made to be more in line with OpenGL naming convention



BindShader Class previously (BindShader.h, BindShader.cpp) are now ShaderUtils.h, ShaderUtil.cpp
==================================================================================================


public members
-----------------

+ class namespace and constructor renamed to ShaderUtil

+ CreateShader function renamed to CreateShaderProgram

+ DeleteShader function renamed to DeleteShaderProgram

+ AddShader function renamed to AddShaderToProgram

+ GetBindedShaderID function renamed to GetShaderProgramID

private members
----------------

+ private member idBindProgram renamed to ProgramID


Example Programs
================

+ example have been updated to work with new shader class