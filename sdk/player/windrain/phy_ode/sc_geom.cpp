//sc_geom.cpp

#include "ode_script.h"

#define CHECK_PGEOM	\
	SCUD_HEAD* pud=(SCUD_HEAD*)lua_touserdata(L,1);	\
	luaL_argcheck(L, (NULL!=pud)&&(UD_NONE!=pud->dwClass), 1, " 'geom' expected");	\
	dGeomID pGeom = *(dGeomID*)(&pud[1]);	\

#define CHECK_XYZ(scale) \
	float x = luaL_checknumber(L, 2) * scale; \
	float y = luaL_checknumber(L, 3) * scale; \
	float z = luaL_checknumber(L, 4) * scale; \


#define PUSH_POS(scale) \
	lua_pushnumber(L, p[0] * scale); \
	lua_pushnumber(L, p[1] * scale); \
	lua_pushnumber(L, p[2] * scale); \


//get enable
int l_geo_get_enable(lua_State *L)
{
	CHECK_PGEOM //����1��Geom (userdata)
	lua_pushboolean(L, dGeomIsEnabled(pGeom));
	return 1;
}

//set enable
int l_geo_set_enable(lua_State *L)
{
	CHECK_PGEOM //����1��Geom (userdata)

	bool bEnable = true;
	if (!lua_isnoneornil(L, 2))
		bEnable = lua_toboolean(L, 2); // enable or disable

	if (bEnable)
		dGeomEnable(pGeom);
	else
		dGeomDisable(pGeom);
	return 0;
}

//get id
int l_geo_get_id(lua_State* L)
{
	CHECK_PGEOM //����1��Geom (userdata)
	lua_pushlightuserdata(L, pGeom);
	return 1;
}


//get position
int l_geo_get_position(lua_State* L)
{
	CHECK_PGEOM //����1��Geom (userdata)
	const dReal* p = dGeomGetPosition(pGeom);
	PUSH_POS(UNIT_SCALE)
	return 3;
}

//set position
int l_geo_set_position(lua_State* L)
{
	CHECK_PGEOM //����1��Geom (userdata)
	CHECK_XYZ(UNIT_SCALE_INV)
	dGeomSetPosition(pGeom, x, y, z);
	dGeomEnable(pGeom);
	return 0;
}

//get quaternion
int l_geo_get_quaternion(lua_State* L)
{
	CHECK_PGEOM //����1��Geom (userdata)
	dQuaternion qua;
	dGeomGetQuaternion (pGeom, qua);
	const dReal* q = qua;
	lua_pushnumber(L, q[1]);
	lua_pushnumber(L, q[2]);
	lua_pushnumber(L, q[3]);
	lua_pushnumber(L, q[0]);
	return 4;
}

//set quaternion
int l_geo_set_quaternion(lua_State* L)
{
	CHECK_PGEOM //����1��Geom (userdata)
	dQuaternion q;
	q[1] = luaL_checknumber(L, 2);
	q[2] = luaL_checknumber(L, 3);
	q[3] = luaL_checknumber(L, 4);
	q[0] = luaL_checknumber(L, 5);
	dGeomSetQuaternion(pGeom, q);
	dGeomEnable(pGeom);
	return 0;
}


//get matrix
int l_geo_get_matrix(lua_State* L)
{
	CHECK_PGEOM //����1��Geom (userdata)
		//const dReal* q = dGeomGetQuaternion(pGeom);
	const dReal* p = dGeomGetPosition(pGeom);
	const dReal* r = dGeomGetRotation(pGeom);
	//const dReal* r = dGeomGetRotation(pGeom->geom);
	static float m[4][4];
	//memcpy(m, r, sizeof(float) * 12);
	m[0][0] = r[0];		m[1][0] = r[1];		m[2][0] = r[2];		m[3][0] = r[3];
	m[0][1] = r[4+0]; m[1][1] = r[4+1];	m[2][1] = r[4+2];	m[3][1] = r[4+3];
	m[0][2] = r[4*2+0]; m[1][2] = r[4*2+1]; m[2][2] = r[4*2+2]; m[3][2] = r[4*2+3];

	m[3][0] = p[0] * UNIT_SCALE; m[3][1] = p[1] * UNIT_SCALE; m[3][2] = p[2] * UNIT_SCALE; m[3][3] = 1.0f;
	lua_pushlightuserdata(L, m);
	return 1;
}