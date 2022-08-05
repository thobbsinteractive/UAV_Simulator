//
//	Basic menu item class for menu nodes
//
#ifndef INC_CMENUITEM_H
#define INC_CMENUITEM_H

#include <vector>
#include <string>

using std::vector;
using std::string;

class CMenuItem
{
public:

	CMenuItem();
	virtual ~CMenuItem();

	string getNodeName();
	void setNodeName(string _sName);

	void setNextNode(CMenuItem *_pNextNode);
	void delNextNode();

	void setPreviousNode(CMenuItem *_pPreNode);
	void delPreviousNode();

	void addSubNode(CMenuItem *_pSubNode);
	
	bool deleteSubNode(string _sName);
	bool deleteSubNode(int index);

	void setFunctionToExecute(void *_pFnc);

	int hasChildren();

private:
	string sName;
	std::vector<CMenuItem> vSubNode;
	CMenuItem *pPreNode;
	CMenuItem *pNextNode;
	void *pFnc;

};
#endif //INC_CDISPLAY_H