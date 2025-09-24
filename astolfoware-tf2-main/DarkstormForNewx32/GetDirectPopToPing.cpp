#include "GetDirectPopToPing.h"
#include "Vars.h"


static const std::map<std::string, int> DataCenterHashMap{
	{"ams",  DC_AMS},
	{"atl",  DC_ATL},
	{"bom",  DC_BOM},
	{"dxb",  DC_DXB},
	{"eat",  DC_EAT},
	//{"mwh",  DC_MWH},
	{"fra",  DC_FRA},
	//{"gnrt", DC_GNRT},
	{"gru",  DC_GRU},
	{"hkg",  DC_HKG},
	{"iad",  DC_IAD},
	{"jnb",  DC_JNB},
	{"lax",  DC_LAX},
	{"lhr",  DC_LHR},
	{"lim",  DC_LIM},
	//{"lux",  DC_LUX},
	{"maa",  DC_MAA},
	{"mad",  DC_MAD},
	//{"man",  DC_MAN},
	//{"okc",  DC_OKC},
	{"ord",  DC_ORD},
	{"par",  DC_PAR},
	{"scl",  DC_SCL},
	{"sea",  DC_SEA},
	{"sgp",  DC_SGP},
	{"sto",  DC_STO},
	{"sto2", DC_STO},
	{"syd",  DC_SYD},
	{"tyo",  DC_TYO},
	{"tyo2", DC_TYO},
	{"tyo1", DC_TYO},
	{"vie",  DC_VIE},
	{"waw",  DC_WAW}
};
void POPID_ToString(SteamNetworkingPOPID popID, char* out)
{
	out[0] = static_cast<char>(popID >> 16);
	out[1] = static_cast<char>(popID >> 8);
	out[2] = static_cast<char>(popID);
	out[3] = static_cast<char>(popID >> 24);
	out[4] = 0;
}

bool IsAllowed(const int nIndex);
#undef max
int __fastcall Hooked_DirectPingToPOP(void* ecx, void* edx, SteamNetworkingPOPID PopID)
{
	if (!Vars::Misc::RegionChanger)
	{
		//return Hook.Original<FN>()(ecx, edx, popID);
		return oDirectPing(ecx, edx, PopID);
	}

	char popIDName[5];
	POPID_ToString(PopID, popIDName);

	//const auto pos = DATA_CENTER_HASH.find(popIDName);
	for (auto& dc : DataCenterHashMap)
	{
		if (dc.first == popIDName && IsAllowed(dc.second))
		{
			return 1;
		}

		if (dc.first == popIDName && !IsAllowed(dc.second))
		{
			return std::numeric_limits<int>::max();
		}
	}


	return oDirectPing(ecx, edx, PopID);
}

bool IsAllowed(int iIndex)
{
	switch (iIndex)
	{
	case (1 << 6):
	case (1 << 25):
	case (1 << 29):
	case (1 << 28):
	{
		return Vars::Misc::EUEast;
		break;
	}
	case (1 << 0):
	case (1 << 13):
	case (1 << 15):
	case (1 << 17):
	case (1 << 21):
	{
		return Vars::Misc::EUWest;
		break;
	}
	case (1 << 18):
	case (1 << 24):
	{
		return Vars::Misc::Asia;
		break;
	}
	case (1 << 3):
	{
		return Vars::Misc::MiddleEast;
		break;
	}
	case (1 << 9):
	{
		return Vars::Misc::HongKong;
		break;
	}
	case (1 << 7):
	case (1 << 27):
	{
		return Vars::Misc::Japan;
		break;
	}
	case (1 << 2):
	case (1 << 16):
	{
		return Vars::Misc::India;
		break;
	}
	case (1 << 26):
	{
		return Vars::Misc::Australia;
		break;
	}
	case (1 << 11):
	{
		return Vars::Misc::Africa;
		break;
	}
	case (1 << 22):
	{
		return Vars::Misc::Chile;
		break;
	}
	case (1 << 14):
	{
		return Vars::Misc::Peru;
		break;
	}
	case (1 << 8):
	{
		return Vars::Misc::Brazil;
		break;
	}
	case (1 << 1):
	case (1 << 10):
	case (1 << 20):
	{
		return Vars::Misc::USEast;
		break;
	}
	case (1 << 4):
	case (1 << 5):
	case (1 << 23):
	{
		return Vars::Misc::USNorthWest;
		break;
	}
	case (1 << 12):
	case (1 << 19):
	{
		return Vars::Misc::USSouthWest;
		break;
	}
	default: return true;
	}

	return true;
}