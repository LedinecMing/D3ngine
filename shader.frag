#version 130
uniform vec2 uni_resolution;
uniform float uni_time;
uniform vec3 uni_angle;
uniform vec3 camera;
uniform float objects[24];

const float LIGHT_POWER = 8.0;

const int OBJECT_LENGTH = 8;
const vec2 BASE_DIST = vec2(1000);
// Object template vars indexes
const int TYPE = 0;
const int POS = 1;
const int SIZE = 4;
const int COLOR = 5;
// Object vars not included in object template
const int NORMAL = 69;

bool cycle = true;
bool is_init_hash = false;
vec2 minDist = vec2(1000);
vec3 getLight()
{	
	if ( cycle )
	{
		return vec3(cos(uni_time), 0.75, sin(uni_time));
	}
	else 
	{
		return vec3(-5.0, 5.0, -5.0);
	}
}

mat2 rotate(float angle) {
	float si = sin(angle);
	float co = sqrt(1-si*si);
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
float plainIntersect(in vec3 ro, in vec3 rd, in vec4 p) 
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
vec3 getObjectParameter(int parameter, int index)
{
	if ( parameter == COLOR )
	{
		return vec3(objects[index + COLOR], objects[index + COLOR + 1], objects[index + COLOR + 2]);
	}
	else if ( parameter == SIZE )
	{
		return vec3(objects[SIZE]);
	}
	else if ( parameter == POS )
	{
		return vec3(objects[index + POS], objects[index + POS + 1], objects[index + POS + 2]);
	}
}
vec3 getObjectParameter(int parameter, int index, vec3 ro, vec3 rd, vec2 dist)
{
	if ( parameter == NORMAL )
	{
		vec3 position = vec3(objects[index+POS], objects[index+POS+1], objects[index+POS+2]);
		float type_form = objects[TYPE];
		if ( type_form == 0 )
		{
			// Sphere normal
			return normalize((ro+rd*dist.x) - position);
		}
		else if ( type_form == 1 )
		{
			// Cube normal
			return normalize((ro+rd*dist.x) - position);
		}
		else if ( type_form == 2 )
		{
			// Plain normal
			return position;
		}
	}
	return vec3(0);
}
vec3 getMinDistObject(vec3 ro, vec3 rd)
{
	vec2 minDist = BASE_DIST;
	vec2 dist;
	int formType;
	float size;	int index = -1;
	for ( int i=0; i<objects.length/OBJECT_LENGTH; i++ )
	{
		vec3 objectCoords = getObjectParameter(POS, i*OBJECT_LENGTH);
		vec3 objectColor = getObjectParameter(COLOR, i*OBJECT_LENGTH);

		size = objects[i*OBJECT_LENGTH+SIZE];
		formType = int(objects[i*OBJECT_LENGTH]);
		/*if ( type_form==2 )
		{
			dist = vec2(plainIntersect(ro, rd, vec4(objectCoords, 1.0)));
			if(dist.x > 0.0 && minDist.x>dist.x)
			{
				minDist = dist;
				index = i;
			}
		}*/
		if ( formType==1 )
		{
			dist = boxIntersection(ro-objectCoords, rd, vec3(0),  vec3(0.0));
			if (dist.x > 0.0 && minDist.x > dist.x)
			{
				minDist = dist;
				index = i;
			} 	
		}
		else if ( formType==0 )
		{
			dist = sphereIntersect(ro, rd, objectCoords, 1);
			if(dist.x > 0.0 && dist.x < minDist.x)
			{	
				minDist = dist;
				index = i;
			}
		}
	}
	return vec3(minDist, index*OBJECT_LENGTH);
}
vec3 colorLight(vec3 color, vec3 normal, vec3 rd)
{
	vec3 light = normalize(getLight());
	float diffuse = max(0.0, dot(light, normal)) * 0.5 + 0.2;
	float specular = max(0.0, pow(dot(reflect(rd, normal), light), LIGHT_POWER));	

	return vec3(color);
}
vec3 rayCast(vec3 ro, vec3 rd)
{
	vec3 collideObject = getMinDistObject(ro, rd);
	vec2 minDist = collideObject.xy;
	int index = int( collideObject.z );
	
	if ( index < 0 || minDist == BASE_DIST)
	{
		return vec3(0, 0, 1);	
	}

	vec3 objectColor = getObjectParameter(COLOR, index);
	vec3 objectNormal = getObjectParameter(NORMAL, index, ro, rd, minDist);
	vec3 readyColor = colorLight(objectColor, objectNormal, rd);

	return readyColor;
}
void main()
{
	vec2 uv = (gl_TexCoord[0].xy - 0.5) * uni_resolution / uni_resolution.y;
	vec3 rayDirection = normalize(vec3(1.0, uv));
	rayDirection.zx *= rotate(-uni_angle.y);
	rayDirection.xy *= rotate(uni_angle.x);
	vec3 render = rayCast(camera, rayDirection);
	gl_FragColor = vec4(render, 1.0);
}