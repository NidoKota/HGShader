#include "ShaderFuncs.hpp"

//虹色のゆらめく円のシェーダー
vec3 RainbowCircleShader(vec3 uv, float t, std::vector<zigsimdata>* zsds)
{
    vec3 result;

    result = rainbow(uv.x * uv.y / 3.f + t / 2.f);
    
    float radius = gradationNoise(uv * 1.5f + t) * 0.5f + 0.5f;

    vec3 centerUV = (uv - 0.5f) * 2.f;
    centerUV.z = 0;

    float width = 0.125f;
    float fade = 0.03f;

    float length = lengthsqr(centerUV);
    float circle = smoothstep(length, length + fade, radius) - smoothstep(length + width, length + width + fade, radius);

    result = screenSynthesis(result, 0.5f);

    result *= circle;

    return result;
}

//ハートシェーダー
vec3 HeartShader(vec3 uv, float t, std::vector<zigsimdata>* zsds)
{
    uv = (uv - 0.5f) * 2.f;

    uv += vec3(0.f, 0.3f);
    uv *= vec3(0.9f, 1.2f);
    
    float radius = 0.3f;
    float fade = 0.05f;
    float length = pow(uv.x, 2.f) + pow(uv.y - sqrt(abs(uv.x)), 2.f) - t * 0.75f;
    float heart = smoothstep(radius + fade, radius, abs(sin(length) * 1.5f));
    
    length *= 2.f;
    float f13 = step(1.f / 3.f, fract(length));
    float f23 = step(2.f / 3.f, fract(length));
    vec3 result = vec3(1.f, 0.63f, 0.63f) * (1.f - f13) +
                  vec3(1.f, 0.3f, 0.4f) * f23 + 
                  vec3(1.f, 0.56f, 0.1f) * f13 * (1.f - f23);
                    
    result *= heart;
    result += 1.f - heart;

    return result;
}

//波シェーダー
vec3 WaveShader(vec3 uv, float t, std::vector<zigsimdata>* zsds)
{
    float colorHeight = 2;

    float wave0 = (sin((t + uv).x * 8.f + t / 2.f) + 0.5f) / 2.f;
    float wave1 = (sin((-0.75f * t + uv).x * -3.f + t / -10.f) + 0.5f) / 2.f;

    float sumWave = ((wave0 + wave1) / 6.f + uv).y;
    float resultWave = smoothstep(sumWave, sumWave + 0.03f, 0.55f);

    vec3 waveColor = (1.f - uv.y * colorHeight) * vec3(0.14f, 0.95f, 0.93f) + uv.y * colorHeight * vec3(0.1f, 0.5f, 1.f);
    vec3 backColor = (1.f - uv.y) * vec3(0.8f, 0.85f, 1.f) + uv.y * vec3(1.f, 0.75f, 0.65f);
    
    return (1.f - resultWave) * backColor + resultWave * waveColor;
}

float circle(vec3 p, float radius)
{
    return length(p) - radius;
}

float polygon(vec3 p, int n, float size)
{
    float a = atan2(p.x, p.y) + M_PI;
    float r = 2.f * M_PI / n;
    return cos(floor(0.5f + a / r) * r - a) * length(p) - size; 
}

float star(vec3 p, int n, float t, float size)
{
    p.z = 0.f;
    float a = 2.f * M_PI / ((float)n * 2.f);
    float aco = cos(a);
    float asi = sin(a);
    vec3 v33[3] = {vec3(aco, -asi), vec3(asi, aco), 0};
    vec3 r = mul3x333(p, v33);
    return (polygon(p, n, size) - polygon(r, n, size) * t) / (1.f - t);
}

float square(vec3 p, vec3 size)
{
    return max(abs(p.x) - size.x, abs(p.y) - size.y);
}

float getShapeRatio(float fade, float t)
{
    t = fract(t);
    float tRoop = fract(t / 5.f) * (1.f + fade) - fade / 2.f;
    return smoothstep(tRoop - 0.05f, tRoop, 0.f) - smoothstep(tRoop, tRoop + 0.05f, 0.f);
}

//形を変えながら回転するシェーダー
vec3 RotationShapesShader(vec3 uv, float t, std::vector<zigsimdata>* zsds)
{
    uv = rotate(uv, smoothstep(0.f, 1.f, fract(t / 3.f)) * 2.5f * M_PI);
    uv -= 0.5f;
    uv *= 2.f;
    
    float squ = square(uv, (vec3)1.f);
    float str = star(uv, 5, 0.5f, 1.f);
    float cir = circle(uv, 1.f);
    float pol = polygon(uv, 3, 0.9f);

    t /= 6.f;
    float fade = 1.f / 4.f;
    return smoothstep(-0.475f, -0.5f, squ * getShapeRatio(fade, t + fade * 0.f) + 
                                      str * getShapeRatio(fade, t + fade * 1.f) + 
                                      cir * getShapeRatio(fade, t + fade * 2.f) +
                                      pol * getShapeRatio(fade, t + fade * 3.f));
}

//ローディングっぽいシェーダー
vec3 LoadingShader(vec3 uv, float t, std::vector<zigsimdata>* zsds)
{
    t *= -5.f;

    float rotateSpeeed = 0.16f;
    float addRotateSpeeed = 0.11f;

    float addRotate = fract(t * addRotateSpeeed);
    float smoothstepDifferential = -6.f * addRotate * addRotate + 6.f * addRotate;

    float inCircleRadius = 0.6f + (smoothstepDifferential / 1.5f) * 0.15f;
    float outCircleRadius = 0.95f - (smoothstepDifferential / 1.5f) * 0.15f;

    float easyAddRotate = smoothstep(0.f, 1.f, addRotate) * 2.f;
    
    vec3 topRotUV = rotate(uv, t * rotateSpeeed * -2.f * (float)M_PI);
    vec3 bottomRotUV = rotate(topRotUV, fract(easyAddRotate) * -2.f * (float)M_PI);

    float topRot = step(topRotUV.x, 0.5f);
    float bottomRot = step(bottomRotUV.x, 0.5f);

    float halfRot1 = 1.f - clamp(topRot + bottomRot, 0.f, 1.f) + topRot;
    float halfRot2 = clamp(topRot - bottomRot, 0.f, 1.f);

    float halfRotSw = step(0.5f, fract(easyAddRotate));
    float rots = clamp(halfRot1 * halfRotSw + halfRot2 * (1.f - halfRotSw), 0.f, 1.f);
    
    float rotsSw = step(1.f, easyAddRotate);
    float allRot =  rots * (1.f - rotsSw) + (1.f - rots) * rotsSw;

    vec3 centerUV = (uv - 0.5f) * 2.f;
    float distance = centerUV.x * centerUV.x + centerUV.y * centerUV.y;
    float fade = 0.03f;
    float inCircle = smoothstep(distance, distance + fade, inCircleRadius * inCircleRadius);
    float outCircle = smoothstep(distance, distance + fade, outCircleRadius * outCircleRadius);

    float colorSw = fract(t * addRotateSpeeed / 4.f) * 4.f;
    float _1St = step(1.f, colorSw);
    float _2St = step(2.f, colorSw);
    float _3St = step(3.f, colorSw);

    vec3 blue = (1.f - _1St) * vec3(0.25f, 0.5f, 0.93f);
    vec3 red = (1.f - _2St) * _1St * vec3(0.89f, 0.26f, 0.2f);
    vec3 yellow = (1.f - _3St) * _2St * vec3(0.95f, 0.72f, 0.02f);
    vec3 green = _3St * vec3(0.2f, 0.64f, 0.3f);

    vec3 color = blue + red + yellow + green;

    return allRot * (outCircle - inCircle) * color;
}

float sphere(vec3 p)
{
    float size = 1.5f;
    return length(mod(p, (vec3)6.f) - (6.f / 2.f)) - size;
}

vec3 getSphereNormal(vec3 p)
{
    float d = 0.0001f;
    return normalize(vec3(sphere(p + vec3(d, 0.f, 0.f)) - sphere(p + vec3(-d, 0.f, 0.f)),
                          sphere(p + vec3(0.f, d, 0.f)) - sphere(p + vec3(0.f, -d, 0.f)),
                          sphere(p + vec3(0.f, 0.f, d)) - sphere(p + vec3(0.f, 0.f, -d))));
}

//宇宙っぽいシェーダー
vec3 SpaceShader(vec3 uv, float t, std::vector<zigsimdata>* zsds)
{
    uv = (uv -0.5f) * 2.f;

    float fov = tan(radians(60.f / 2.f));
    vec3 lightDir = vec3(0.5f);

    vec3 color;
    
    vec3 comPos = vec3(0.f, 0.f, -t);
    vec3 comForword = vecRotateZ(vecRotateY(vecRotateX(vec3(0.f, 0.f, 1.f), sin(t / 14.f)), cos(t / 14.f)), -sin(t / 14.f));
    vec3 comUp = vecRotateZ(vecRotateY(vecRotateX(vec3(0.f, 1.f, 0.f), sin(t / 14.f)), cos(t / 14.f)), -sin(t / 14.f));
    vec3 comRight = cross(comForword, comUp);
    
    vec3 ray = normalize(comForword + comRight * uv.x * fov + comUp * uv.y * fov);
    
    float distance = 0.f;
    float rLength = 0.f;
    vec3 rPos = comPos;
    for(int i = 0; i < 30; i++)
    {
        distance = sphere(rPos);
        rLength += distance;
        rPos = comPos + ray * rLength;
    }

    float sw = 1.f - step(0.001f, abs(distance));
    float fade = 0.01f;
    float dps = rLength / 40.f;
    float a = fract(t / 10.f) * (fade + 1.f) + fade;
    color = clamp(dot(lightDir, getSphereNormal(rPos)), 0.f, 1.f) * 0.3f + 0.3f + (step(a - fade, dps) - step(a + fade, dps)) * 0.3f; 
    color *= sw;

    return floor(color * 9.f) / 9.f;
}

float boxFunc(vec3 p)
{
    vec3 q = abs(p);
    return length(max(q - vec3(0.5f, 0.f, 0.5f), 0.f)) - 0.3f;
}

float torusFunc(vec3 p)
{
    vec3 t = vec3(0.65f, 0.2f);
    vec3 r = vec3(length(vec3(p.x, p.y)) - t.x, p.z);
    return length(r) - t.y;
}

float blockFunc(vec3 p)
{
    float d1 = torusFunc(p);
    float d2 = boxFunc(p);
    return max(-d1, d2);
}

vec3 getBlockNormal(vec3 p)
{
    float d = 0.0001f;
    return normalize(vec3(blockFunc(p + vec3(d, 0.f, 0.f)) - blockFunc(p + vec3(-d, 0.f, 0.f)),
                          blockFunc(p + vec3(0.f, d, 0.f)) - blockFunc(p + vec3(0.f, -d, 0.f)),
                          blockFunc(p + vec3(0.f, 0.f, d)) - blockFunc(p + vec3(0.f, 0.f, -d))));
}

//ブロックが回転するシェーダー
vec3 BlockShader(vec3 uv, float t, std::vector<zigsimdata>* zsds)
{
    t *= 5.f;
    uv = (uv -0.5f) * 2.f;

    float fov = tan(radians(60.f / 2.f));

    vec3 color;
    
    vec3 comPos = vecRotateZ(vecRotateY(vec3(1.f), t / 3.f), t / 6.f) * 2.f;
    vec3 comForword = -normalize(comPos);
    vec3 comUp = vecRotateZ(comForword, M_PI / 2.f);
    vec3 comRight = cross(comForword, comUp);
    
    vec3 lightDir = -comForword;

    vec3 ray = normalize(comForword + comRight * uv.x * fov + comUp * uv.y * fov);
    
    float distance = 0.f;
    float rLength = 0.f;
    vec3 rPos = comPos;
    for(int i = 0; i < 30; i++)
    {
        distance = blockFunc(rPos);
        rLength += distance;
        rPos = comPos + ray * rLength;
    }

    float sw = 1.f - step(0.001f, abs(distance));
    color = clamp(dot(lightDir, getBlockNormal(rPos)), 0.f, 1.f) * 0.4f + 0.1f;
    color *= sw;

    return floor(color * 30.f) / 30.f;
}

float longBoxFunc(vec3 p, vec4 qu)
{
    vec3 q = abs(quatXvec3(qu, p));
    return length(max(q - vec3(0.4f, 0.8f, 0.1f), 0.f)) - 0.05f;
}

vec3 getLongBoxNormal(vec3 p, vec4 qu)
{
    float d = 0.0001f;
    return normalize(vec3(longBoxFunc(p + vec3(d, 0.f, 0.f), qu) - longBoxFunc(p + vec3(-d, 0.f, 0.f), qu),
                          longBoxFunc(p + vec3(0.f, d, 0.f), qu) - longBoxFunc(p + vec3(0.f, -d, 0.f), qu),
                          longBoxFunc(p + vec3(0.f, 0.f, d), qu) - longBoxFunc(p + vec3(0.f, 0.f, -d), qu)));
}

vec4 x90rot = angleAxis2Quat(M_PI / 2.f, vec3(1, 0, 0));

//スマホでZigSimを使って回転させるシェーダー
vec3 PhoneShader(vec3 uv, float t, std::vector<zigsimdata>* zsds)
{
    uv = (uv -0.5f) * 2.f;

    float fov = tan(radians(60.f / 2.f));

    vec3 color;
    
    vec3 comPos;

    comPos = vec3(0, 0, 2.25f);

    vec3 comForword = normalize(vec3(0) - comPos);
    vec3 comUp = quatXvec3(x90rot, comForword);
    vec3 comRight = normalize(cross(comForword, comUp));
    
    vec3 lightDir = -comForword;

    vec3 ray = normalize(comForword + comRight * uv.x * fov + comUp * uv.y * fov);
    
    float distance = 0.f;
    float rLength = 0.f;
    vec3 rPos = comPos;
    vec4 qu;
    if(zsds->size() > 0) qu = quatXquat(quatInverse(zsds->operator[](0)._sensorData.quaternion), x90rot);
    else qu = angleAxis2Quat(M_PI * t / 5.f, vec3(1, 0, 0));

    for(int i = 0; i < 25; i++)
    {
        distance = longBoxFunc(rPos, qu);
        rLength += distance;
        rPos = comPos + ray * rLength;
    }

    float sw = 1.f - step(0.001f, abs(distance));
    color = clamp(dot(lightDir, getLongBoxNormal(rPos, qu)), 0.f, 1.f) * 0.4f + 0.1f;
    color *= sw;

    return floor(color * 20.f) / 20.f;
}

int isNum(int num, int value)
{
    return step(num, value) - step(num + 1.f, value);
}

vec3 Race(vec3 uv, float t, std::vector<zigsimdata>* zsds)
{
    float rotate = 0;
    vec3 pos = 0;

    if(zsds->size() > 0)
    {
        sensordata sd = zsds->operator[](0)._sensorData;
        rotate = radians(sd.compass);
        pos = vec3(sd.touch.x, (sin(t / 10.f) * 0.5f + 0.5f) * 0.25f + 0.15f, -sd.touch.y);
    } 
    else 
    {
        pos = vec3(sin(t / 50.f) * 0.45f, 0.3f * (sin(t / 10.f) * 0.5f + 0.5f) + 0.05f, cos(t / 50.f) * 0.45f);
        rotate = atan2(-pos.x, -pos.z);
    }

    float defY = uv.y;

    float calY = pos.y / (1 - defY);

    uv -= 0.5f;
    uv = quatXvec3(angleAxis2Quat(rotate, vec3(0, 0, 1)), uv);
    uv *= calY;
    uv += 0.5f;
    uv.z = 0;

    uv += vec3(pos.x, pos.z, 0);

    int mapCount = 10;
    int map[10][10] = 
    {
        {0, 2, 1, 1, 4, 0, 0, 0, 0, 0},
        {0, 1, 0, 0, 1, 0, 0, 6, 0, 0},
        {0, 3, 4, 0, 3, 1, 1, 4, 6, 0},
        {0, 0, 1, 6, 0, 0, 0, 3, 4, 0},
        {2, 1, 1, 4, 6, 0, 0, 0, 1, 0},
        {1, 1, 6, 3, 1, 4, 6, 0, 1, 0},
        {3, 1, 6, 6, 6, 3, 4, 0, 1, 0},
        {6, 1, 1, 1, 4, 6, 1, 0, 1, 0},
        {0, 1, 1, 6, 1, 1, 1, 1, 5, 0},
        {0, 3, 1, 1, 1, 5, 6, 0, 0, 0},
    };    

    uv *= mapCount;
    vec3 flooruv = floor(uv);
    uv = fract(uv);
    
    uv -= 0.5f;
    uv *= 2.f;
    uv += 0.065f;
    
    vec3 sand = vec3(0.4, 0.3, 0.07);
    vec3 flat = vec3(0.9, 0.84, 0.29);

    float treef = step(circle(uv, 0.2f), 1.f);
    vec3 tree = treef * vec3(0.04, 0.4, 0.12) + (1.f - treef) * sand;

    vec3 upharf = step(0.f, uv);
    upharf.z = 0;
    vec3 dwharf = 1.f - upharf;
    dwharf.z = 0;

    float circ = step(circle(uv, 0.4f), 1.f);
    
    float leupf = clamp(circ + upharf.x + dwharf.y, 0.f, 1.f);
    vec3 leup = leupf * flat + (1.f - leupf) * sand;
    float ledwf = clamp(circ + upharf.x + upharf.y, 0.f, 1.f);
    vec3 ledw = ledwf * flat + (1.f - ledwf) * sand;
    float riupf = clamp(circ + dwharf.x + dwharf.y, 0.f, 1.f);
    vec3 riup = riupf * flat + (1.f - riupf) * sand;
    float ridwf = clamp(circ + dwharf.x + upharf.y, 0.f, 1.f);
    vec3 ridw = ridwf * flat + (1.f - ridwf) * sand;

    int mapValueX = mapCount - (int)flooruv.y - 1;
    int mapValueY = (int)flooruv.x;
    int isIn = step(0, mapValueX) * step(mapValueX, mapCount - 1) * step(0, mapValueY) * step(mapValueY, mapCount - 1);
    int mapValue = map[clamp(mapValueX, 0, mapCount - 1)][clamp(mapValueY, 0, mapCount - 1)];
    
    float skyY = 0.15f * (1.f - pos.y) + 0.85f;
    float skyf = step(skyY, defY);
    vec3 sky = skyf * vec3(0, 0.55, 0.72);

    vec3 result = 
       isNum(0, mapValue) * sand
     + isNum(1, mapValue) * flat
     + isNum(2, mapValue) * leup
     + isNum(3, mapValue) * ledw
     + isNum(4, mapValue) * riup
     + isNum(5, mapValue) * ridw
     + isNum(6, mapValue) * tree;
    
    result *= isIn;
    result += (1 - isIn) * sand;

    return result * step(defY, skyY) + sky * (1.f - step(defY, skyY));
}