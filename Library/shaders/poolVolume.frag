#version 330 core

in vec2 texcoord;
out vec3 fragColor;
uniform sampler2DArray texScene;
uniform sampler2D texLinearDepth;

const vec3 waterAbsorption = vec3(0.8,0.97,0.99);

const vec2 Poisson32[32] = vec2[](
    vec2(-0.975402, -0.0711386),
    vec2(-0.920347, -0.41142),
    vec2(-0.883908, 0.217872),
    vec2(-0.884518, 0.568041),
    vec2(-0.811945, 0.90521),
    vec2(-0.792474, -0.779962),
    vec2(-0.614856, 0.386578),
    vec2(-0.580859, -0.208777),
    vec2(-0.53795, 0.716666),
    vec2(-0.515427, 0.0899991),
    vec2(-0.454634, -0.707938),
    vec2(-0.420942, 0.991272),
    vec2(-0.261147, 0.588488),
    vec2(-0.211219, 0.114841),
    vec2(-0.146336, -0.259194),
    vec2(-0.139439, -0.888668),
    vec2(0.0116886, 0.326395),
    vec2(0.0380566, 0.625477),
    vec2(0.0625935, -0.50853),
    vec2(0.125584, 0.0469069),
    vec2(0.169469, -0.997253),
    vec2(0.320597, 0.291055),
    vec2(0.359172, -0.633717),
    vec2(0.435713, -0.250832),
    vec2(0.507797, -0.916562),
    vec2(0.545763, 0.730216),
    vec2(0.56859, 0.11655),
    vec2(0.743156, -0.505173),
    vec2(0.736442, -0.189734),
    vec2(0.843562, 0.357036),
    vec2(0.865413, 0.763726),
    vec2(0.872005, -0.927)
);

const vec2 Poisson64[64] = vec2[](
    vec2(-0.934812, 0.366741),
    vec2(-0.918943, -0.0941496),
    vec2(-0.873226, 0.62389),
    vec2(-0.8352, 0.937803),
    vec2(-0.822138, -0.281655),
    vec2(-0.812983, 0.10416),
    vec2(-0.786126, -0.767632),
    vec2(-0.739494, -0.535813),
    vec2(-0.681692, 0.284707),
    vec2(-0.61742, -0.234535),
    vec2(-0.601184, 0.562426),
    vec2(-0.607105, 0.847591),
    vec2(-0.581835, -0.00485244),
    vec2(-0.554247, -0.771111),
    vec2(-0.483383, -0.976928),
    vec2(-0.476669, -0.395672),
    vec2(-0.439802, 0.362407),
    vec2(-0.409772, -0.175695),
    vec2(-0.367534, 0.102451),
    vec2(-0.35313, 0.58153),
    vec2(-0.341594, -0.737541),
    vec2(-0.275979, 0.981567),
    vec2(-0.230811, 0.305094),
    vec2(-0.221656, 0.751152),
    vec2(-0.214393, -0.0592364),
    vec2(-0.204932, -0.483566),
    vec2(-0.183569, -0.266274),
    vec2(-0.123936, -0.754448),
    vec2(-0.0859096, 0.118625),
    vec2(-0.0610675, 0.460555),
    vec2(-0.0234687, -0.962523),
    vec2(-0.00485244, -0.373394),
    vec2(0.0213324, 0.760247),
    vec2(0.0359813, -0.0834071),
    vec2(0.0877407, -0.730766),
    vec2(0.14597, 0.281045),
    vec2(0.18186, -0.529649),
    vec2(0.188208, -0.289529),
    vec2(0.212928, 0.063509),
    vec2(0.23661, 0.566027),
    vec2(0.266579, 0.867061),
    vec2(0.320597, -0.883358),
    vec2(0.353557, 0.322733),
    vec2(0.404157, -0.651479),
    vec2(0.410443, -0.413068),
    vec2(0.413556, 0.123325),
    vec2(0.46556, -0.176183),
    vec2(0.49266, 0.55388),
    vec2(0.506333, 0.876888),
    vec2(0.535875, -0.885556),
    vec2(0.615894, 0.0703452),
    vec2(0.637135, -0.637623),
    vec2(0.677236, -0.174291),
    vec2(0.67626, 0.7116),
    vec2(0.686331, -0.389935),
    vec2(0.691031, 0.330729),
    vec2(0.715629, 0.999939),
    vec2(0.8493, -0.0485549),
    vec2(0.863582, -0.85229),
    vec2(0.890622, 0.850581),
    vec2(0.898068, 0.633778),
    vec2(0.92053, -0.355693),
    vec2(0.933348, -0.62981),
    vec2(0.95294, 0.156896)
);

void main(void) 
{
	float depth = texture(texLinearDepth, texcoord).r;
	fragColor = texture(texScene, vec3(texcoord, int(clamp(depth/10.0,1.0,5.0)))).rgb; 
	
	/*fragColor = vec3(0,0,0);
	int samples = 1;
	
	for(int i=0; i<64; ++i)
	{
		vec2 sampleTexcoord = texcoord + Poisson64[i] * clamp(depth/1000.0, 0.0, 0.005);
		float sampleDepth = texture(texLinearDepth, sampleTexcoord).r;
		if(sampleDepth >= depth)
		{
			vec3 color = vec3(pow(waterAbsorption.r, depth), pow(waterAbsorption.g, depth), pow(waterAbsorption.b, depth)) * texture(texScene, sampleTexcoord).rgb;
			fragColor += mix(color, vec3(0.2,0.5,0.5), clamp(depth*depth/1000.0, 0.0, 1.0));
			++samples;
		}
	}*/
	/*
	for(int i=0; i<64; ++i)
	{
		vec2 sampleTexcoord = texcoord - Poisson64[i] * depth/800.0;
		float sampleDepth = texture(texLinearDepth, sampleTexcoord).r;
		if(sampleDepth >= depth)
		{
			color += texture(texScene, sampleTexcoord).rgb;
			++samples;
		}
	}
	
	for(int i=0; i<64; ++i)
	{
		vec2 sampleTexcoord = texcoord + vec2(Poisson64[i].x, -Poisson64[i].y) * depth/1200.0;
		float sampleDepth = texture(texLinearDepth, sampleTexcoord).r;
		if(sampleDepth >= depth)
		{
			color += texture(texScene, sampleTexcoord).rgb;
			++samples;
		}
	}
	
	for(int i=0; i<64; ++i)
	{
		vec2 sampleTexcoord = texcoord + vec2(-Poisson64[i].x, Poisson64[i].y) * depth/600.0;
		float sampleDepth = texture(texLinearDepth, sampleTexcoord).r;
		if(sampleDepth >= depth)
		{
			color += texture(texScene, sampleTexcoord).rgb;
			++samples;
		}
	}*/
	
	//fragColor /= float(samples);
	//fragColor *= vec3(pow(waterAbsorption.r, depth), pow(waterAbsorption.g, depth), pow(waterAbsorption.b, depth));
	fragColor = fragColor + vec3(0.2,0.5,0.5) * clamp(depth*depth/1000.0, 0.0, 1.0);
}