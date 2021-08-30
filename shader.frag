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
const int REFLECTIVITY = 7;
const int COLOR = 10;
// Object vars which are not included in object template
const int NORMAL = 69;
const int OBJECT_LENGTH = 13;

const int MAX_REFLECT = 512;
const vec4 GOT_SKY = vec4(-1);

uniform float lightPower;
uniform vec2 baseDistation;

uniform float objects[39];

uniform bool getInfo;

bool lightCycle = false;
vec2 minDist = vec2(1000);
vec3 light = normalize(vec3(-0.5, 0.75, -1.0));

mat2 rot(float a) {
	float s = sin(a);
	float c = cos(a);
	return mat2(c, -s, s, c);
}

vec2 sphIntersect( in vec3 rayOrigin, in vec3 rayDirection, in vec3 ce, float ra ) 
{
	vec3 oc = rayOrigin - ce;
	float b = dot( oc, rayDirection ); 
	float c = dot( oc, oc ) - ra*ra; 
	float h = b*b - c; 
	if( h<0.0 ) return vec2(-1.0); 
	// no intersection 
	h = sqrt( h ); 
	return vec2( -b-h, -b+h ); 
}

vec2 boxIntersection(in vec3 rayOrigin, in vec3 rayDirection, in vec3 rad, out vec3 oN)  {
	vec3 m = 1.0 / rayDirection;
	vec3 normal = m * rayOrigin;
	vec3 k = abs(m) * rad;
	vec3 t1 = -normal - k;
	vec3 t2 = -normal + k;
	float tN = max(max(t1.x, t1.y), t1.z);
	float tF = min(min(t2.x, t2.y), t2.z);
	if(tN > tF || tF < 0.0) return vec2(-1.0);
	oN = -sign(rayDirection) * step(t1.yzx, t1.xyz) * step(t1.zxy, t1.xyz);
	return vec2(tN, tF);
}

float plaIntersect(in vec3 rayOrigin, in vec3 rayDirection, in vec4 p) {
	return -(dot(rayOrigin, p.xyz) + p.w) / dot(rayDirection, p.xyz);
}

vec3 getSky(vec3 rayDirection) {
	vec3 color = vec3(0.3, 0.6, 1.0);
	vec3 sun = vec3(0.95, 0.9, 1.0);
	sun *= max(0.0, pow(dot(rayDirection, light), lightPower));
	color *= max(0.0, dot(light, vec3(0.0, 0.0, -1.0)));
	return clamp(sun + color, 0.0, 1.0);
}
vec3 getObjectPos(int index)
{
	return vec3(objects[index + POS], objects[index + POS + 1], objects[index + POS + 2]);
}
vec3 getObjectSize(int index)
{
	return vec3(objects[index + SIZE], objects[index + SIZE + 1], objects[index + SIZE + 2]);
}
vec3 getObjectColor(int index)
{
	return vec3(objects[index + COLOR], objects[index + COLOR + 1], objects[index + COLOR + 2]);
}
float getObjectReflectivity(int index)
{
	return objects[index + REFLECTIVITY];
}

vec3 getObjectNormal(int objectIndex, vec3 rayOrigin, vec3 rayDirection, vec2 distation)
{
	vec3 position = vec3(objects[objectIndex+POS], objects[objectIndex+POS+1], objects[objectIndex+POS+2]);
	float type_form = objects[objectIndex+TYPE];
	if ( type_form < 2 )
	{
		// Sphere and cube normal
		return normalize((rayOrigin+rayDirection*distation.x) - position);
	}
	else if ( type_form == 2 )
	{
		// Plane normal
		return vec3(0, 0, 1);
	}
	return vec3(0);
}
vec4 castRay(inout vec3 rayOrigin, inout vec3 rayDirection) {
	vec4 color;
	vec2 minDistation = vec2(baseDistation);
	vec2 distation;
	vec3 normal;
	int objectIndex = -1;
	for ( int i=0; i<objects.length/OBJECT_LENGTH; i++ )
	{
		int readyIndex = i*OBJECT_LENGTH;
		vec3 objectSize = getObjectSize(readyIndex);
		if ( objectSize == vec3(0))
		{
			continue;
		}
		vec3 objectCoords = getObjectPos(readyIndex);
		vec3 objectColor = getObjectColor(readyIndex);
		int objectForm = int(objects[readyIndex+TYPE]);
		switch ( objectForm )
		{
			case 2:
				distation = vec2(plaIntersect(rayOrigin, rayDirection, normalize(vec4(objectCoords, 1.0))));
				break;
			case 1:
				vec3 boxn;
				distation = boxIntersection(rayOrigin-objectCoords, rayDirection, objectSize,  boxn);
				break;
			case 0:
				distation = sphIntersect(rayOrigin, rayDirection, objectCoords, objectSize.x);
				break;
		}
		if(distation.x > 0.0 && minDistation.x>distation.x )
		{
			minDistation = distation;
			objectIndex = i;
			color = vec4(objectColor, getObjectReflectivity(objectIndex*OBJECT_LENGTH));
			normal = getObjectNormal( i*OBJECT_LENGTH, rayOrigin, rayDirection, minDistation);
		}
	}
	if(minDistation.x == baseDistation) return vec4(-1.0);
	float diffuse = max(0.0, dot(light, normal));
	float specular = max(0.0, pow(dot(reflect(rayDirection, normal), light), 32.0));
	vec3 shade = mix(diffuse, specular, 0.5);
	color.rgb *= mix(shade, vec3(1.0), color.a);
	rayOrigin += rayDirection * (minDistation.x - 0.001);
	rayDirection = reflect(rayDirection, normal);
	return color;
}

vec3 traceRay(vec3 rayOrigin, vec3 rayDirection) {
	vec3 color = vec3(dot(light, vec3(0.0, 0.0, -1.0)));
	float reflectivity = 1.0;
	for(int i = 0; i < MAX_REFLECT; i++)
	{
		vec4 refCol = castRay(rayOrigin, rayDirection);
		if(refCol.x == -1.0) return mix(color, color * getSky(rayDirection), reflectivity);
		vec3 lightDir = light;
		vec3 shadowRo = rayOrigin;
		if(castRay(shadowRo, lightDir).x != -1.0) refCol.rgb *= vec3(refCol.a);
		color *= mix(vec3(1.0), refCol.rgb, reflectivity);
		reflectivity *= refCol.a;
	}
	return color;
}

void main() {
	vec2 uv = (gl_TexCoord[0].xy - 0.5) * uni_resolution / uni_resolution.y;
	vec3 rayOrigin = cameraPosition;
	vec3 rayDirection = normalize(vec3(1.0, uv));
	rayDirection.zx *= rot(cameraAngle.y);
	rayDirection.xy *= rot(cameraAngle.x);
	light = normalize(vec3(sin(uni_time * 0.2), 0.75, cos(uni_time * 0.2) - 0.95));
	vec3 color = traceRay(rayOrigin, rayDirection);
	color.r = pow(color.r, 0.45);
	color.g = pow(color.g, 0.45);
	color.b = pow(color.b, 0.45);
	gl_FragColor = vec4(color, 1.0);
}
