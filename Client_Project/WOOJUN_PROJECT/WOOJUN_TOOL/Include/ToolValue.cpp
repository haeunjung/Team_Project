#include "stdafx.h"
#include "ToolValue.h"

DEFINITION_SINGLE(CToolValue)

CToolValue::CToolValue()
{

}

CToolValue::~CToolValue()
{
	SAFE_RELEASE(pPlayerObj);
}