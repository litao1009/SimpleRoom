#include "SCADMeshTool.h"


#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>
#include <set>

typedef	boost::filesystem::path		PathType;
typedef	boost::filesystem::ofstream	OutStream;
typedef	boost::filesystem::ifstream	InStream;


class	STriIndex
{
public:

	STriIndex():PntIndex(0),NorIndex(0),TexIndex(0) {}

	int PntIndex,NorIndex,TexIndex;
};


bool		SCADMeshTool::SaveToObj(SCADMeshPtr meshPtr, const char* strFileName)
{
	PathType	ph(strFileName);
	OutStream	ofs(ph, std::ios::trunc);

	PntsVector		pv;
	TrianglesVector	tv;
	meshPtr->GetSolidMesh(pv, tv);


	for (  auto& curPnt : pv )
	{
		ofs << "v "		<< curPnt.X	<< " " << curPnt.Y	<< " " << curPnt.Z	<< std::endl;
		ofs << "vt "	<< curPnt.U	<< " " << curPnt.V	<< std::endl;
		ofs << "vn "	<< curPnt.nX << " " << curPnt.nY	<< " " << curPnt.nZ	<< std::endl;
		ofs << std::endl;
	}

	for ( auto& curTriangle : tv )
	{
		ofs << "f "	<< std::get<0>(curTriangle)+1 << "/" << std::get<0>(curTriangle)+1 << "/" << std::get<0>(curTriangle)+1 << " ";
		ofs			<< std::get<1>(curTriangle)+1 << "/" << std::get<1>(curTriangle)+1 << "/" << std::get<1>(curTriangle)+1 << " ";
		ofs			<< std::get<2>(curTriangle)+1 << "/" << std::get<2>(curTriangle)+1 << "/" << std::get<2>(curTriangle)+1 << std::endl;
	}

	ofs.close();

	return true;
}

SCADMeshPtr	SCADMeshTool::OpenObjFile(const char* strFileName)
{

	static boost::regex	rgPnt("\\s*v(?=\\s)\\s+(-?\\d+(?:\\.\\d+)?)\\s+(-?\\d+(?:\\.\\d+)?)\\s+(-?\\d+(?:\\.\\d+)?)\\s*");
	static boost::regex	rgNormal("\\s*vn\\s+(-?\\d+(?:\\.\\d+)?)\\s+(-?\\d+(?:\\.\\d+)?)\\s+(-?\\d+(?:\\.\\d+)?)\\s*");
	static boost::regex	rgTexUV("\\s*vt\\s+(-?\\d+(?:\\.\\d+)?)\\s+(-?\\d+(?:\\.\\d+)?)\\s*");
	static boost::regex rgTri("\\s*f\\s+(\\d+)/(\\d+)/(\\d+)\\s+(\\d+)/(\\d+)/(\\d+)\\s+(\\d+)/(\\d+)/(\\d+)");

	PathType	ph(strFileName);
	InStream	ifs(ph);

	if ( !ifs.is_open() )
	{
		return SCADMeshPtr();
	}

	PntsVector		pnts;
	PntsVector		nors;
	PntsVector		uvs;
	TrianglesVector tris;
	std::set<int>	pIdSet;
	std::set<int>	nIdSet;
	std::set<int>	tIdSet;
	std::vector<STriIndex>	triIndexs;

	boost::smatch what;
	std::string strLine;
	while( getline(ifs, strLine))
	{
		if ( boost::regex_match(strLine, what, rgPnt) )
		{
			SPntInfo pnt;
			pnt.X = boost::lexical_cast<double>(what[1].str());
			pnt.Y = boost::lexical_cast<double>(what[2].str());
			pnt.Z = boost::lexical_cast<double>(what[3].str());
			pnts.push_back(pnt);
		}
		else if ( boost::regex_match(strLine, what, rgNormal) )
		{
			SPntInfo nor;
			nor.nX = boost::lexical_cast<double>(what[1].str());
			nor.nY = boost::lexical_cast<double>(what[2].str());
			nor.nZ = boost::lexical_cast<double>(what[3].str());
		}
		else if ( boost::regex_match(strLine, what, rgTexUV) )
		{
			SPntInfo uv;
			uv.U = boost::lexical_cast<double>(what[1].str());
			uv.V = boost::lexical_cast<double>(what[2].str());
		}
		else if ( boost::regex_match(strLine, what, rgTri) )
		{
			int p1 = boost::lexical_cast<int>(what[1].str())-1;
			int t1 = boost::lexical_cast<int>(what[2].str())-1;
			int n1 = boost::lexical_cast<int>(what[3].str())-1;
			int p2 = boost::lexical_cast<int>(what[4].str())-1;
			int t2 = boost::lexical_cast<int>(what[5].str())-1;
			int n2 = boost::lexical_cast<int>(what[6].str())-1;
			int p3 = boost::lexical_cast<int>(what[7].str())-1;
			int t3 = boost::lexical_cast<int>(what[8].str())-1;
			int n3 = boost::lexical_cast<int>(what[9].str())-1;

			STriIndex tri1,tri2,tri3;
			tri1.PntIndex = p1;
			tri1.TexIndex = t1;
			tri1.NorIndex = n1;

			tri2.PntIndex = p2;
			tri2.TexIndex = t2;
			tri2.NorIndex = n2;

			tri3.PntIndex = p3;
			tri3.TexIndex = t3;
			tri3.NorIndex = n3;

			triIndexs.push_back(tri1);
			triIndexs.push_back(tri2);
			triIndexs.push_back(tri3);

			pIdSet.insert(p1);
			pIdSet.insert(p2);
			pIdSet.insert(p3);

			nIdSet.insert(n1);
			nIdSet.insert(n2);
			nIdSet.insert(n3);

			tIdSet.insert(t1);
			tIdSet.insert(t2);
			tIdSet.insert(t3);

			tris.emplace_back(p1, p2, p3);
		}
	}
	ifs.close();

	if ( pnts.size()!=nors.size() || nors.size()!=uvs.size() || pnts.size()!=uvs.size() )
	{
		return SCADMeshPtr();
	}


	for ( std::size_t i=0; i<triIndexs.size(); ++i )
	{
		int pntIndex = triIndexs[i].PntIndex;
		int norIndex = triIndexs[i].NorIndex;
		int texIndex = triIndexs[i].TexIndex;

		if ( 0==pIdSet.count(pntIndex) || 0==nIdSet.count(norIndex) || 0==tIdSet.count(texIndex) )
		{
			return SCADMeshPtr();
		}		

		pnts[pntIndex].nX = nors[norIndex].nX;
		pnts[pntIndex].nY = nors[norIndex].nY;
		pnts[pntIndex].nZ = nors[norIndex].nZ;

		pnts[texIndex].U = uvs[texIndex].U;
		pnts[texIndex].V = uvs[texIndex].V;
	}
	nors.clear();
	uvs.clear();

	return SCADMeshPtr();
}