#version 130
uniform vec2 uni_resolution;
uniform float uni_time;

// Camera data
uniform vec3 cameraSize;
uniform vec3 cameraAngle;
uniform vec3 cameraPosition;

// Object template vars indexes
const int TYPE = 0;
const int POS = 1;
const int SIZE = 4;
const int ANGLE = 7;
const int COLOR = 10;
// Object vars which are not included in object template
const int NORMAL = 69;
const int OBJECT_LENGTH = 13;

const vec3 GOT_SKY = vec3(-1, -1, -1);

uniform float lightPower;
uniform vec2 baseDistation;

uniform float objects[39];

uniform bool getInfo;

bool lightCycle = true;
vec2 minDist = vec2(1000);

vec3 getLight()
{	
	if ( lightCycle )
	{
		return vec3(cos(uni_time), 0.75, sin(uni_time));
	}
	else 
	{
		return vec3(-0.5, 0.75, 1);
	}
}

mat2 rotate(float angle) {
	float si = sin(angle);
	float co = cos(angle);
	return mat2(co, -si, si, co);
}

vec2 sphereIntersect( in vec3 ro, in vec3 rd, in vec3 ce, float ra ) 
{
	vec3 oc = ro - ce;
	float b = dot( oc, rd ); 
	float c = dot( oc, oc ) - ra*ra; 
	float h = b*b - c; 
	if( h<0.0 ) return vec2(-1.0); 
	// no intersection 
	h = sqrt( h ); 
	return vec2( -b-h, -b+h ); 
}
// axis aligned box centered at the origin, with size boxSize
float planeIntersect(in vec3 ro, in vec3 rd, in vec4 p) 
{
	return -(dot(ro, p.xyz) + p.w) / dot(rd, p.xyz);
}
vec2 boxIntersection( in vec3 ro, in vec3 rd, vec3 boxSize, vec3 outNormal ) 
{ 
	vec3 m = 1.0/rd;
	// can precompute if traversing a set of aligned boxes vec3 
	vec3 n = m*ro; 
	// can precompute if traversing a set of aligned boxes vec3 
	vec3 k = abs(m)*boxSize; 
	vec3 t1 = -n - k; 
	vec3 t2 = -n + k; 
	float tN = max( max( t1.x, t1.y ), t1.z ); 
	float tF = min( min( t2.x, t2.y ), t2.z ); 
	if( tN>tF || tF<0.0)
	{
		return vec2(-1.0);
	}
	// no intersection 
	outNormal = -sign(rd)*step(t1.yzx,t1.xyz)*step(t1.zxy,t1.xyz); 
	return vec2( tN, tF ); 
}
// triangle degined by vertices v0, v1 and  v2
vec3 triangleIntersect( in vec3 ro, in vec3 rd, in vec3 v0, in vec3 v1, in vec3 v2 )
{
    vec3 v1v0 = v1 - v0;
    vec3 v2v0 = v2 - v0;
    vec3 rov0 = ro - v0;
    vec3  n = cross( v1v0, v2v0 );
    vec3  q = cross( rov0, rd );
    float d = 1.0/dot( rd, n );
    float u = d*dot( -q, v2v0 );
    float v = d*dot(  q, v1v0 );
    float t = d*dot( -n, rov0 );
    if( u<0.0 || u>1.0 || v<0.0 || (u+v)>1.0 ) t = -1.0;
    return vec3( t, u, v );
}

vec3 getObjectParameter(int parameter, int index)
{
	if ( parameter == COLOR )
	{
		return vec3(objects[index + COLOR], objects[index + COLOR + 1], objects[index + COLOR + 2]);
	}
	else if ( parameter == SIZE )
	{
		return vec3(objects[index + SIZE], objects[index + SIZE+1], objects[index + SIZE+2]);
	}
	else if ( parameter == POS )
	{
		return vec3(objects[index + POS], objects[index + POS + 1], objects[index + POS + 2]);
	}
}
vec3 getObjectParameter(int parameter, int objectIndex, vec3 rayOrigin, vec3 rayDirection, vec2 distation)
{
	if ( parameter == NORMAL )
	{
		vec3 position = vec3(objects[objectIndex+POS], objects[objectIndex+POS+1], objects[objectIndex+POS+2]);
		float type_form = objects[objectIndex+TYPE];
		if ( type_form == 0 )
		{
			// Sphere normal
			return normalize((rayOrigin+rayDirection*distation.x) - position);
		}
		else if ( type_form == 1 )
		{
			// Cube normal
			//return normalize((rayOrigin+rayDirection*distation.x) - position);
			vec3 boxnormal;
			return boxnormal;
		}
		else if ( type_form == 2 )
		{
			// Plane normal
			return vec3(0, 0, 1);
		}
	}
	return vec3(0);
}
vec3 getMinDistObject(vec3 rayOrigin, vec3 rayDirection)
{
	vec3 objectSize;
	vec3 objectCoords;
	vec3 objectColor;

	vec2 minDistation = baseDistation;
	vec2 distation;

	int objectForm;
	int objectIndex;

	for ( int i=0; i<objects.length/OBJECT_LENGTH; i++ )
	{
		objectSize = getObjectParameter(SIZE, i*OBJECT_LENGTH);
		if ( objectSize == vec3(0))
		{
			return vec3(baseDistation, -1);
		}
		objectCoords = getObjectParameter(POS, i*OBJECT_LENGTH);
		objectColor = getObjectParameter(COLOR, i*OBJECT_LENGTH);
		objectForm = int(objects[i*OBJECT_LENGTH+TYPE]);
		switch ( objectForm )
		{
			case 2:
				distation = vec2(planeIntersect(rayOrigin, rayDirection, normalize(vec4(objectCoords*10, 1.0))));
				break;
			case 1:
				distation = boxIntersection(rayOrigin-objectCoords, rayDirection, objectSize,  vec3(0.0));
				break;
			case 0:
				distation = sphereIntersect(rayOrigin, rayDirection, objectCoords, objectSize.x);
				break;
		}
		if(distation.x > 0.0 && minDistation.x>distation.x )
		{
			minDistation = distation;
			objectIndex = i;
		}
	}
	rayOrigin += rayDirection * (minDistation.x - 0.001);
	return vec3(minDistation, objectIndex*OBJECT_LENGTH);
}
vec3 gammaCorrect(vec3 color)
{
	return vec3( pow(color, vec3(0.45)));
}
vec3 colorLight(vec3 color, vec3 normal, vec3 rd)
{
	vec3 light = normalize(getLight());
	float diffuse = max(0.0, dot(light, normal));
	float specular = max(0.0, pow(dot(reflect(rd, normal), light), lightPower));	

	return gammaCorrect((mix(diffuse, specular, 0.5)*color));
}
vec3 castRay(vec3 rayOrigin, vec3 rayDirection)
{
	vec3 collideObject = getMinDistObject(rayOrigin, rayDirection);
	vec2 minDistation = collideObject.xy;
	int objectIndex = int( collideObject.z );
	
	if ( objectIndex < 0 || minDistation == baseDistation)
	{
		return vec3(-1, -1, -1);	
	}
	vec3 objectColor = getObjectParameter(COLOR, objectIndex);
	vec3 objectNormal = getObjectParameter(NORMAL, objectIndex, rayOrigin, rayDirection, minDistation);
	vec3 readyColor = colorLight(objectColor, objectNormal, rayDirection);
	return readyColor;
}
vec3 rayTrace(vec3 rayOrigin, vec3 rayDirection)
{
	vec3 color = castRay(rayOrigin, rayDirection);

	if ( color == GOT_SKY )
	{
		return vec3(0, 0, 1);
	}

	vec3 lightDirection = normalize( getLight() );

	if ( castRay(getMinDistObject(rayOrigin, rayOrigin+rayDirection)[0]*rayDirection, lightDirection*vec3(1)) == GOT_SKY )
	{
		color*=0.0;
	}
	return color;
}
void main()
{
	if ( getInfo )
	{
		gl_FragColor = vec4( int(getMinDistObject(cameraPosition, vec3(0, 0, -1))[0]), 0, 0, 1);
		return;
	}
	vec2 uv = (gl_TexCoord[0].xy - 0.5) * uni_resolution / uni_resolution.y;
	vec3 rayDirection = normalize(vec3(1.0, uv));
	rayDirection.zx *= rotate(cameraAngle.y);
	rayDirection.xy *= rotate(cameraAngle.x);
	vec3 render = rayTrace(cameraPosition, rayDirection);
	gl_FragColor = vec4(render, 1);
}
