#version 130
uniform vec2 uni_resolution;
uniform float uni_time;
uniform vec3 uni_angle;
uniform vec3 camera;
mat4 spheres= mat4(vec4(0.0, 0.0, 0.0, 0.0), vec4(0.0, 0.0, 0.0, 0.0), vec4(0.0, 0.0, 0.0, 0.0), vec4(5.0, 0.0, 0.0, 0.6));
mat2x4 cubes = mat2x4(vec4(0.0, 0.0, 0.0, 0.2), vec4(5.0, 2.0, -1.0, 0.1));
mat4x3 empty = mat4x3(vec3(0.0), vec3(0.0), vec3(0.0), vec3(0.0));
// sphere of size ra centered at point ce
bool cycle = false;
bool is_init_hash = false;
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
vec2 sphIntersect( in vec3 ro, in vec3 rd, in vec3 ce, float ra ) 
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
float plaIntersect(in vec3 ro, in vec3 rd, in vec4 p) {
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
vec3 rayCast(vec3 ro, vec3 rd)
{
	vec4 sphere, cube;
	vec3 col = vec3(0.0);
	vec3 planeNormal = vec3(0.0, 0.0, -1.0);
	vec2 minDist = vec2(1000.0);
	vec3 normal = vec3(0.0);
	vec2 dist = vec2(plaIntersect(ro, rd, vec4(planeNormal, 1.0)));
	if(dist.x > 0.0 && minDist.x>dist.x)
	{
		// Нормаль
		normal = planeNormal;
		minDist = dist;
	}
	for(int i=0; i<spheres.length; i++)
	{
		sphere = spheres[i];
		if ( sphere==vec4(0.0))
		{
			continue;
		}
		vec2 point = sphIntersect(ro, rd, sphere.xyz, sphere.w);
		if(point.x < 0.0 || point.x>minDist.x)
		{	
			continue;
		} 
		minDist = point;
		//  нормаль
		normal = normalize((ro+rd*point.x)-sphere.xyz);
		minDist = point;
	}
	for(int i=0; i<cubes.length; i++)
	{
		cube = cubes[i];
		if ( cube==vec4(0.0))
		{
			continue;
		}
		vec2 point = boxIntersection(ro-cube.xyz, rd, vec3(1.0),  vec3(0.0));
		if (point.x < 0.0 || minDist.x<point.x)
		{
			continue;
		} 
		// Нормаль
		vec3 nbl;
		normal = normalize((ro+rd*point.x)-cube.xyz);		
		minDist = point;
	}
	if ( normal == vec3(0.0) )
	{
		return vec3(0.0);
	}
	vec3 light = normalize(getLight());
	float diffuse = max(0.0, dot(light, normal)) * 0.5 + 0.2;
	float specular = max(0.0, pow(dot(reflect(rd, normal), light), 4.0));
	col = vec3(diffuse + specular);
	
	if ( minDist==vec2(1000.0) )
	{
		return vec3(0, 0, 128);
	}

	return col;
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