
#version 330 core

in vec3 vertex;
out vec4 fragColor;

uniform uint frameCounter;

uniform int width;
uniform int height;
uniform int depth;

uniform bool hasData;

uniform samplerBuffer triangles;
uniform samplerBuffer BVHnodes;
uniform samplerBuffer materials;

//灯光三角形列表: x=累积面积(CDF), y=三角形索引, z=三角形面积
uniform samplerBuffer lights;
uniform int lightTriangleCount;
uniform float lightTotalArea;

uniform sampler2DArray textureMapsArrayTex;

uniform sampler2D lastFrame;
uniform samplerCube cubemap;

uniform vec3 eye;
uniform mat4 cameraRotate;
uniform mat4 projection;

//常量定义

#define PI              3.1415926
#define INF             10000000
#define SIZE_TRIANGLE   9
#define SIZE_MATERIAL   7
#define SIZE_BVHNODE    4
#define EPSILON 0.000001


// 三角形数据格式
struct Triangle {
    vec3 p1, p2, p3;    // 顶点坐标
    vec3 n1, n2, n3;    // 顶点法线
    vec3 texCoord1, texCoord2, texCoord3;
};

// BVH节点
struct BVHNode {
    int left;           // 左子树
    int right;          // 右子树
    int n;              // 包含三角形数目
    int index;          // 三角形索引
    vec3 AA, BB;        // 碰撞盒
};

// 物体表面材质定义
struct Material {
    vec3 emissive;          // 作为光源时的发光颜色
    vec3 baseColor;
    float subsurface;
    float metallic;
    float specular;
    float specularTint;
    float roughness;
    float anisotropic;
    float sheen;
    float sheenTint;
    float clearcoat;
    float clearcoatGloss;
    int baseColorTexId;
    int metalnessTexId;
    int normalTexId;
    int emissiveTexId;
};

// 光线
struct Ray {
    vec3 startPoint;
    vec3 direction;
};

// 光线求交结果
struct HitResult {
    bool isHit;             // 是否命中
    bool isInside;          // 是否从内部命中
    float distance;         // 与交点的距离
    vec3 hitPoint;          // 光线命中点

    vec3 normal;            
    vec3 tangent;
    vec3 bitangent;         // 命中点切线空间
    vec2 texCoord; //命中点纹理坐标

    vec3 viewDir;           // 击中该点的光线的方向
    Material material;      // 命中点的表面材质
};


/*
 * 生成随机向量，依赖于 frameCounter 帧计数器
 * 代码来源：https://blog.demofox.org/2020/05/25/casual-shadertoy-path-tracing-1-basic-camera-diffuse-emissive/
*/

uint seed = uint(
    uint((vertex.x * 0.5 + 0.5) * width)  * uint(1973) + 
    uint((vertex.y * 0.5 + 0.5) * height) * uint(9277) + 
    uint(frameCounter) * uint(26699)) | uint(1);

uint wang_hash(inout uint seed) {
    seed = uint(seed ^ uint(61)) ^ uint(seed >> uint(16));
    seed *= uint(9);
    seed = seed ^ (seed >> 4);
    seed *= uint(0x27d4eb2d);
    seed = seed ^ (seed >> 15);
    return seed;
}
 
float rand() {
    return float(wang_hash(seed)) / 4294967296.0;
}

void getTangent(vec3 N, inout vec3 tangent, inout vec3 bitangent) {
    vec3 helper = vec3(1, 0, 0);
    if(abs(N.x)>0.999) helper = vec3(0, 0, 1);
    bitangent = normalize(cross(N, helper));
    tangent = normalize(cross(N, bitangent));
} //计算法线空间


/*
 * 余弦加权半球采样(重要性采样)
 * 令 pdf(wi) = cos(theta) / PI,则对漫反射 BRDF 而言 f_r * cos / pdf 几乎为常数,
 * 相比半球均匀采样可以显著降低方差。采样方式为 Malley 方法:先在单位圆盘均匀取点再投影到半球。
 */
vec3 SampleCosineHemisphere() {
    float r = sqrt(rand());
    float phi = 2.0 * PI * rand();
    float z = sqrt(max(0.0, 1.0 - r*r));
    return vec3(r * cos(phi), r * sin(phi), z);
}


// 将向量 v 投影到 N 的法向半球
vec3 toNormalHemisphere(vec3 v, vec3 N) {
    vec3 helper = vec3(1, 0, 0);
    if(abs(N.x)>0.999) helper = vec3(0, 0, 1);
    vec3 tangent = normalize(cross(N, helper));
    vec3 bitangent = normalize(cross(N, tangent));
    return v.x * tangent + v.y * bitangent + v.z * N;
}

// 采样天空盒
vec3 sampleCube(vec3 v){
    return texture(cubemap, v).rgb;
}


// 获取第 i 下标的三角形
Triangle getTriangle(int i) {
    int offset = i * SIZE_TRIANGLE;
    Triangle t;

    // 顶点坐标
    t.p1 = texelFetch(triangles, offset + 0).xyz;
    t.p2 = texelFetch(triangles, offset + 1).xyz;
    t.p3 = texelFetch(triangles, offset + 2).xyz;
    // 法线
    t.n1 = texelFetch(triangles, offset + 3).xyz;
    t.n2 = texelFetch(triangles, offset + 4).xyz;
    t.n3 = texelFetch(triangles, offset + 5).xyz;

    t.texCoord1 = texelFetch(triangles, offset + 6).xyz;
    t.texCoord2 = texelFetch(triangles, offset + 7).xyz;
    t.texCoord3 = texelFetch(triangles, offset + 8).xyz;

    return t;
}

// 获取第 i 下标的三角形的材质
Material getMaterial(int i, inout HitResult res) {
    Material m;

    int offset = i * SIZE_MATERIAL;

    m.emissive = texelFetch(materials, offset + 0).xyz;
    m.baseColor = texelFetch(materials, offset + 1).xyz;
    vec3 param1 = texelFetch(materials, offset + 2).xyz;
    vec3 param2 = texelFetch(materials, offset + 3).xyz;
    vec3 param3 = texelFetch(materials, offset + 4).xyz;
    vec3 param4 = texelFetch(materials, offset + 5).xyz;
    vec3 param5 = texelFetch(materials, offset + 6).xyz;

    m.subsurface = param1.x;
    m.metallic = param1.y;
    m.specular = param1.z;
    m.specularTint = param2.x;
    m.roughness = max(param2.y,0.0001);
    m.anisotropic = param2.z;
    m.sheen = param3.x;
    m.sheenTint = param3.y;
    m.clearcoat = param3.z;
    m.clearcoatGloss = param4.x;
    m.baseColorTexId = int(param4.y);
    m.metalnessTexId = int(param4.z);
    m.normalTexId = int(param5.x);
    m.emissiveTexId = int(param5.y);

     // 反射贴图
    if (m.baseColorTexId >= 0)
    {
       vec4 col = texture(textureMapsArrayTex, vec3(res.texCoord, m.baseColorTexId));
       m.baseColor *= pow(col.rgb, vec3(2.2));
    }

    // 金属贴图
    if (m.metalnessTexId >= 0)
    {
       vec2 matRgh = texture(textureMapsArrayTex, vec3(res.texCoord, m.metalnessTexId)).bg;
       m.metallic = matRgh.x;
       m.roughness = max(matRgh.y * matRgh.y, 0.001);
    }

    //法线贴图
    if (m.normalTexId >= 0)
    {
       vec3 texNormal = texture(textureMapsArrayTex, vec3(res.texCoord, m.normalTexId)).rgb;

       texNormal.y = 1.0 - texNormal.y;
       texNormal = normalize(texNormal * 2.0 - 1.0);

       res.normal = normalize(res.tangent * texNormal.x + res.bitangent * texNormal.y + res.normal * texNormal.z);
 
    }

    //自发光贴图
    if (m.emissiveTexId >= 0)
        m.emissive = pow(texture(textureMapsArrayTex, vec3(res.texCoord, m.emissiveTexId)).rgb, vec3(2.2));

    return m;
}

// 获取第 i 下标的 BVHNode 对象
BVHNode getBVHNode(int i) {
    BVHNode node;

    // 左右子树
    int offset = i * SIZE_BVHNODE;
    ivec3 childs = ivec3(texelFetch(BVHnodes, offset + 0).xyz);
    ivec3 leafInfo = ivec3(texelFetch(BVHnodes, offset + 1).xyz);
    node.left = int(childs.x);
    node.right = int(childs.y);
    node.n = int(leafInfo.x);
    node.index = int(leafInfo.y);

    // 包围盒
    node.AA = texelFetch(BVHnodes, offset + 2).xyz;
    node.BB = texelFetch(BVHnodes, offset + 3).xyz;

    return node;
}

// 光线和三角形求交 
HitResult hitTriangle(Triangle triangle, Ray ray) {

    HitResult res;
    res.distance = INF;
    res.isHit = false;
    res.isInside = false;

    vec3 p1 = triangle.p1;
    vec3 p2 = triangle.p2;
    vec3 p3 = triangle.p3;

    vec3 s = ray.startPoint - p1;
    vec3 e1 = p2-p1;
    vec3 e2 = p3-p1;

    vec3 s1 = cross(ray.direction, e2);
    vec3 s2 = cross(s, e1);

    float s1e1 = dot(s1, e1);
    float t = dot(s2, e2)/s1e1;
    float a = dot(s1, s)/s1e1;
    float b = dot(s2, ray.direction)/s1e1;
    float c = 1-a-b;   //Möller-Trumbore算法求交
    // 从三角形背后（模型内部）击中

    vec3 N = normalize(cross(p2-p1, p3-p1));    // 法向量
    if (abs(dot(N, ray.direction)) < 0.00001f) return res;

    if (t < 0.0005f) return res;    // 如果三角形在光线背面

    // 命中，封装返回结果
    if (t >= EPSILON && a >= EPSILON && b >= EPSILON && c >= EPSILON) {

        res.isHit = true;
        res.distance = t;
        res.viewDir = ray.direction;
        res.hitPoint = ray.startPoint + t * ray.direction;
        res.texCoord = a * triangle.texCoord1.xy + b * triangle.texCoord2.xy + c * triangle.texCoord3.xy;
        vec3 normal = normalize(a* triangle.n1 + b* triangle.n2 + c* triangle.n3);
        res.normal = dot(normal, ray.direction) <= 0.0 ? normal : -normal;
        getTangent(res.normal, res.tangent, res.bitangent); //计算切线空间

        float alpha = (-(res.hitPoint.x-p2.x)*(p3.y-p2.y) + (res.hitPoint.y-p2.y)*(p3.x-p2.x)) / (-(p1.x-p2.x-0.00005)*(p3.y-p2.y+0.00005) + (p1.y-p2.y+0.00005)*(p3.x-p2.x+0.00005));
        float beta  = (-(res.hitPoint.x-p3.x)*(p1.y-p3.y) + (res.hitPoint.y-p3.y)*(p1.x-p3.x)) / (-(p2.x-p3.x-0.00005)*(p1.y-p3.y+0.00005) + (p2.y-p3.y+0.00005)*(p1.x-p3.x+0.00005));
        float gama  = 1.0 - alpha - beta;
        res.texCoord = alpha * triangle.texCoord1.xy + beta * triangle.texCoord2.xy + gama * triangle.texCoord3.xy; //插值计算坐标
        
    }

    return res;
}

// 和 aabb 盒子求交，没有交点则返回 -1
float hitAABB(Ray r, vec3 AA, vec3 BB) {
    vec3 invdir = 1.0 / r.direction;

    vec3 f = (BB - r.startPoint) * invdir;
    vec3 n = (AA - r.startPoint) * invdir;

    vec3 tmax = max(f, n);
    vec3 tmin = min(f, n);

    float t1 = min(tmax.x, min(tmax.y, tmax.z));
    float t0 = max(tmin.x, max(tmin.y, tmin.z));

    return (t1 >= t0) ? ((t0 > 0.0) ? (t0) : (t1)) : (-1);
}

// ----------------------------------------------------------------------------- //

// 暴力遍历数组下标范围 [l, r] 求最近交点
HitResult hitArray(Ray ray, int l, int r) {
    HitResult res;
    res.isHit = false;
    res.distance = INF;
    for(int i=l; i<=r; i++) {
        Triangle triangle = getTriangle(i);
        HitResult r = hitTriangle(triangle, ray);
        if(r.isHit && r.distance<res.distance) {
            res = r;
            res.material = getMaterial(i, res);
        }
    }
    return res;
}

// 遍历 BVH 求交
HitResult hitBVH(Ray ray) {
    HitResult res;
    res.isHit = false;
    res.distance = INF;

    // 栈
    int stack[256];
    int sp = 0;
    int guard = 0; //遍历保护: 防御异常BVH数据导致死循环/栈溢出

    stack[sp++] = 1;
    while(sp>0) {
        int top = stack[--sp];
        if(++guard > 65536) break;              //单条光线最多访问节点数
        if(top <= 0 || top > (1<<22)) continue; //非法节点下标
        BVHNode node = getBVHNode(top);
        
        // 是叶子节点，遍历三角形，求最近交点
        if(node.n>0) {
            int L = node.index;
            int R = node.index + node.n - 1;
            HitResult r = hitArray(ray, L, R);
            if(r.isHit && r.distance<res.distance) res = r;
            continue;
        }
        
        // 和左右盒子 AABB 求交
        float d1 = INF; // 左盒子距离
        float d2 = INF; // 右盒子距离
        if(node.left>0) {
            BVHNode leftNode = getBVHNode(node.left);
            d1 = hitAABB(ray, leftNode.AA, leftNode.BB);
        }
        if(node.right>0) {
            BVHNode rightNode = getBVHNode(node.right);
            d2 = hitAABB(ray, rightNode.AA, rightNode.BB);
        }

        // 在最近的盒子中搜索(同时用当前最近距离剪枝)
        if(d1>0 && d2>0) {
            if(d1<d2) { // d1<d2, 左边先
                if(d2 < res.distance) stack[sp++] = node.right;
                if(d1 < res.distance) stack[sp++] = node.left;
            } else {    // d2<d1, 右边先
                if(d1 < res.distance) stack[sp++] = node.left;
                if(d2 < res.distance) stack[sp++] = node.right;
            }
        } else if(d1>0) {   // 仅命中左边
            if(d1 < res.distance) stack[sp++] = node.left;
        } else if(d2>0) {   // 仅命中右边
            if(d2 < res.distance) stack[sp++] = node.right;
        }
    }

    return res;
}

/* 
 * 遮挡测试:只判断 [0, maxDist) 内是否有交点, 命中即返回(阴影光线专用)
 * 相比 hitBVH 的最近交点搜索, 提前退出能显著减少节点访问
 */
bool occluded(Ray ray, float maxDist) {
    int stack[128];
    int sp = 0;
    int guard = 0;

    stack[sp++] = 1;
    while(sp>0) {
        int top = stack[--sp];
        if(++guard > 65536) break;
        if(top <= 0 || top > (1<<22)) continue;
        BVHNode node = getBVHNode(top);

        if(node.n>0) {
            int L = node.index;
            int R = node.index + node.n - 1;
            for(int i=L; i<=R; i++) {
                HitResult r = hitTriangle(getTriangle(i), ray);
                if(r.isHit && r.distance < maxDist) return true;
            }
            continue;
        }

        if(node.left>0) {
            BVHNode ln = getBVHNode(node.left);
            float d = hitAABB(ray, ln.AA, ln.BB);
            if(d>0 && d<maxDist) stack[sp++] = node.left;
        }
        if(node.right>0) {
            BVHNode rn = getBVHNode(node.right);
            float d = hitAABB(ray, rn.AA, rn.BB);
            if(d>0 && d<maxDist) stack[sp++] = node.right;
        }
    }

    return false;
}

float sqr(float x) { 
    return x*x; 
}

float SchlickFresnel(float u) {
    float m = clamp(1-u, 0, 1);
    float m2 = m*m;
    return m2*m2*m; // pow(m,5)
}

float GTR1(float NdotH, float a) {
    if (a >= 1) return 1/PI;
    float a2 = a*a;
    float t = 1 + (a2-1)*NdotH*NdotH;
    return (a2-1) / (PI*log(a2)*t);
}

float GTR2(float NdotH, float a) {
    float a2 = a*a;
    float t = 1 + (a2-1)*NdotH*NdotH;
    return a2 / (PI * t*t);
}

float GTR2_aniso(float NdotH, float HdotX, float HdotY, float ax, float ay) {
    return 1 / (PI * ax*ay * sqr( sqr(HdotX/ax) + sqr(HdotY/ay) + NdotH*NdotH ));
}

float smithG_GGX(float NdotV, float alphaG) {
    float a = alphaG*alphaG;
    float b = NdotV*NdotV;
    return 1 / (NdotV + sqrt(a + b - a*b));
}

float smithG_GGX_aniso(float NdotV, float VdotX, float VdotY, float ax, float ay) {
    return 1 / (NdotV + sqrt( sqr(VdotX*ax) + sqr(VdotY*ay) + sqr(NdotV) ));
}

vec3 BRDF_Evaluate(vec3 V, vec3 N, vec3 L, vec3 X, vec3 Y, in Material material) {
    float NdotL = dot(N, L);
    float NdotV = dot(N, V);
    if(NdotL < 0 || NdotV < 0) return vec3(0);

    vec3 H = normalize(L + V);
    float NdotH = dot(N, H);
    float LdotH = dot(L, H);

    // 各种颜色
    vec3 Cdlin = material.baseColor;
    float Cdlum = 0.3 * Cdlin.r + 0.6 * Cdlin.g  + 0.1 * Cdlin.b;
    vec3 Ctint = (Cdlum > 0) ? (Cdlin/Cdlum) : (vec3(1));   
    vec3 Cspec = material.specular * mix(vec3(1), Ctint, material.specularTint);
    vec3 Cspec0 = mix(0.08*Cspec, Cdlin, material.metallic); // 0° 镜面反射颜色
    vec3 Csheen = mix(vec3(1), Ctint, material.sheenTint);   // 织物颜色

    // 漫反射
    float Fd90 = 0.5 + 2.0 * LdotH * LdotH * material.roughness;
    float FL = SchlickFresnel(NdotL);
    float FV = SchlickFresnel(NdotV);
    float Fd = mix(1.0, Fd90, FL) * mix(1.0, Fd90, FV);

    // 次表面散射
    float Fss90 = LdotH * LdotH * material.roughness;
    float Fss = mix(1.0, Fss90, FL) * mix(1.0, Fss90, FV);
    float ss = 1.25 * (Fss * (1.0 / (NdotL + NdotV) - 0.5) + 0.5);

    // 镜面反射 -- 各向异性
    float aspect = sqrt(1.0 - material.anisotropic * 0.9);
    float ax = max(0.001, sqr(material.roughness)/aspect);
    float ay = max(0.001, sqr(material.roughness)*aspect);
    float Ds = GTR2_aniso(NdotH, dot(H, X), dot(H, Y), ax, ay);
    float FH = SchlickFresnel(LdotH);
    vec3 Fs = mix(Cspec0, vec3(1), FH);
    float Gs;
    Gs  = smithG_GGX_aniso(NdotL, dot(L, X), dot(L, Y), ax, ay);
    Gs *= smithG_GGX_aniso(NdotV, dot(V, X), dot(V, Y), ax, ay);

    // 清漆
    float Dr = GTR1(NdotH, mix(0.1, 0.001, material.clearcoatGloss));
    float Fr = mix(0.04, 1.0, FH);
    float Gr = smithG_GGX(NdotL, 0.25) * smithG_GGX(NdotV, 0.25);

    // sheen
    vec3 Fsheen = FH * material.sheen * Csheen;
    
    vec3 diffuse = (1.0/PI) * mix(Fd, ss, material.subsurface) * Cdlin + Fsheen;
    vec3 specular = Gs * Fs * Ds;
    vec3 clearcoat = vec3(0.25 * Gr * Fr * Dr * material.clearcoat);

    return diffuse * (1.0 - material.metallic) + specular + clearcoat;
}

/*
 * 按面积累积概率(CDF)从灯光三角形列表中抽一个三角形
 * 用小面积的亮光源时,靠 BSDF 采样"碰巧"打中光源的概率极低,噪声很大;
 * 直接按概率抽取光源三角形就能显著降低方差
 */
int pickLightTriangle() {
    if (lightTriangleCount <= 0) return -1;

    float u = rand() * lightTotalArea;
    int lo = 0;
    int hi = lightTriangleCount - 1;
    while (lo < hi) {
        int mid = (lo + hi) / 2;
        if (texelFetch(lights, mid).x >= u) hi = mid;
        else lo = mid + 1;
    }
    return int(texelFetch(lights, lo).y);
}

// 取第 i 个三角形的自发光颜色(材质数组与三角形一一对应)
vec3 lightEmissive(int i) {
    return texelFetch(materials, i * SIZE_MATERIAL + 0).xyz;
}

/*
 * 下一事件估计(NEE):直接对灯光做重要性采样
 * 返回该方向的直接光照贡献, pdfLight 为该方向的立体角概率密度(供多重重要性采样使用)
 */
vec3 sampleDirectLight(HitResult hit, vec3 V, out float pdfLight, out vec3 lightDir) {
    pdfLight = 0.0;
    lightDir = vec3(0);

    int triIndex = pickLightTriangle();
    if (triIndex < 0) return vec3(0);

    Triangle lt = getTriangle(triIndex);
    float r1 = sqrt(rand());
    float r2 = rand();
    vec3 lightPoint = (1.0 - r1) * lt.p1 + r1 * (1.0 - r2) * lt.p2 + r1 * r2 * lt.p3; //三角形上均匀取点

    vec3 L = lightPoint - hit.hitPoint;
    float dist2 = dot(L, L);
    float dist = sqrt(dist2);
    if (dist < EPSILON) return vec3(0);
    L /= dist;

    vec3 lightNormal = normalize(cross(lt.p2 - lt.p1, lt.p3 - lt.p1));
    float cosSurface = dot(hit.normal, L);
    float cosLight = -dot(lightNormal, L);
    if (cosSurface <= 0.0 || cosLight <= 0.0) return vec3(0);

    //可见性:向光源打阴影光线(遮挡测试,命中即退出)
    Ray shadowRay;
    shadowRay.startPoint = hit.hitPoint;
    shadowRay.direction = L;
    if (occluded(shadowRay, dist - 0.001)) return vec3(0); //被遮挡

    //三角形面积均匀采样的密度 1/lightTotalArea,转换成该方向的立体角密度
    pdfLight = dist2 / (cosLight * lightTotalArea);
    lightDir = L;

    vec3 f_r = BRDF_Evaluate(V, hit.normal, L, hit.tangent, hit.bitangent, hit.material);
    vec3 Le = lightEmissive(triIndex);
    // f_r * Le * (cosSurface*cosLight/dist2) / (1/lightTotalArea)
    return f_r * Le * cosSurface * cosLight * lightTotalArea / dist2;
}

// 路径追踪
vec3 pathTracing(HitResult hit, int maxBounce) {
    vec3 Lo = vec3(0);      // 最终的颜色
    vec3 history = vec3(1); // 递归积累的颜色

    for(int bounce=0; bounce<maxBounce; bounce++) {
        vec3 V = -hit.viewDir;
        vec3 N = hit.normal;

        // 1.直接光照:对灯光做重要性采样(下一事件估计)
        float pdfLight = 0.0;
        vec3 lightDir = vec3(0);
        vec3 direct = sampleDirectLight(hit, V, pdfLight, lightDir);
        if (pdfLight > 0.0) {
            float pdfBSDF = max(0.0, dot(N, lightDir)) / PI; //出射方向若由BSDF采样的概率密度
            float weight = pdfLight / (pdfLight + pdfBSDF);  //平衡启发式,避免重复计算同一路径
            Lo += history * direct * weight;
        }

        // 2.间接光照:按余弦加权(重要性采样)采样出射方向
        vec3 L = toNormalHemisphere(SampleCosineHemisphere(), hit.normal);
        float cosine_i = max(0.0, dot(L, N));
        float pdf = cosine_i / PI;
        if (pdf <= 0.0) break;

        vec3 f_r = BRDF_Evaluate(V, N, L, hit.tangent, hit.bitangent, hit.material);
        vec3 weightBSDF = f_r * cosine_i / pdf;

        Ray randomRay;
        randomRay.startPoint = hit.hitPoint;
        randomRay.direction = L;
        HitResult newHit = hitBVH(randomRay);

        // 未命中
        if(!newHit.isHit) {
            vec3 skyColor = sampleCube(randomRay.direction);
            Lo += history * weightBSDF * skyColor;
            break;
        }

        // 命中灯光三角形:用与下一事件估计一致的权重,防止同一份光被重复计算
        vec3 Le = newHit.material.emissive;
        if (dot(Le, Le) > 0.0) {
            float pdfLightAtHit = 0.0;
            if (lightTriangleCount > 0) {
                float cosLight = max(0.0, dot(newHit.normal, -L));
                if (cosLight > 0.0)
                    pdfLightAtHit = newHit.distance * newHit.distance / (cosLight * lightTotalArea);
            }
            float weight = (pdfLightAtHit > 0.0) ? (pdf / (pdf + pdfLightAtHit)) : 1.0;
            Lo += history * weightBSDF * Le * weight;
        }

        // 递归(步进)
        hit = newHit;
        history *= weightBSDF;  // 累积颜色
    }

    return Lo;
}


void main() {

    Ray ray;
    
    ray.startPoint = eye;
    vec2 AA = vec2((rand()-0.5)/float(width), (rand()-0.5)/float(height));
    vec4 dir = cameraRotate * projection* vec4(vertex.xy+AA, 1.0, 1.0);
    ray.direction = normalize(dir.xyz);
    vec3 color;

    if(hasData){
         // primary hit
        HitResult firstHit = hitBVH(ray);
        
        if(!firstHit.isHit) {
            color = vec3(0);
            color = sampleCube(ray.direction);
        } else {
            vec3 Le = firstHit.material.emissive;
            vec3 Li = pathTracing(firstHit, depth);
            color = Le + Li;
        }  
    
        // 和上一帧混合
        vec3 lastColor = texture2D(lastFrame, vertex.xy*0.5+0.5).rgb;
        color = mix(lastColor, color, 1.0/float(frameCounter+uint(1)));

    }
    else{
        color = sampleCube(ray.direction);
    }
    // 输出
    fragColor = vec4(color, 1.0);
}
