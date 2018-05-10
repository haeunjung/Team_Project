#include "07.Component/Script.h"

WOOJUN_USING

class CMessageBox : public CScript
{

public:
	bool Init() override;
	void Input(float _fTime) override;
	void Update(float _fTime) override;
	CMessageBox* Clone() override;
private:
	void CreateButton();
public:
	void OkButton(CGameObject * _pObj, float _fTime);
public:
	CMessageBox();
	CMessageBox(const CMessageBox& _MessageBox);
	~CMessageBox();
};
