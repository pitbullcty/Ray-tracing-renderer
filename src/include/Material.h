#ifndef __RIAL__
#define __RIAL__

#include <QVector3D>
#include <QJsonObject>

struct MaterialEncoded {
    QVector3D emissive;      // 自发光参数
    QVector3D baseColor;     // 颜色
    QVector3D param1;        // (subsurface, metallic, specular)
    QVector3D param2;        // (specularTint, roughness, anisotropic)
    QVector3D param3;        // (sheen, sheenTint, clearcoat)
    QVector3D param4;        // (clearcoatGloss,baseColorTexId, metalnessTexId) 记录材质类型
    QVector3D param5;       //(normalTexId, emissiveTexId, 0)
    MaterialEncoded() = default;
};

struct ModelMaterial
{
    QVector3D emissive;  // 作为光源时的发光颜色
    QVector3D baseColor; //基本颜色
    float subsurface; //次表面，控制漫反射形状
    float metallic;   //金属度
    float specular;   //高光强度
    float specularTint; //高光染色
    float roughness; //粗糙度
    float anisotropic; //各项异性
    float sheen; //光泽度
    float sheenTint; //光泽色
    float clearcoat; //清漆强度
    float clearcoatGloss; //清漆光泽度

    ModelMaterial() {
        emissive = QVector3D(0, 0, 0); 
        baseColor = QVector3D(1, 1, 1);
        subsurface = 0.0; 
        metallic = 0.0;   
        specular = 0.0;  
        specularTint = 0.0;
        roughness = 0.5; 
        anisotropic = 0.0; 
        sheen = 0.0; 
        sheenTint = 0.0; 
        clearcoat = 0.0;
        clearcoatGloss = 0.0; 
    }

    QJsonObject toJson(){
        QJsonObject material;
        QJsonObject emissive;
        QJsonObject baseColor;
        emissive.insert("x", this->emissive.x());
        emissive.insert("y", this->emissive.y());
        emissive.insert("z", this->emissive.z());
        material.insert("emissive", emissive);
        baseColor.insert("r", this->baseColor.x());
        baseColor.insert("g", this->baseColor.y());
        baseColor.insert("b", this->baseColor.z());
        material.insert("baseColor", baseColor);
        material.insert("subsurface", subsurface);
        material.insert("metallic", metallic);
        material.insert("specular", specular);
        material.insert("specularTint", specularTint);
        material.insert("roughness", roughness);
        material.insert("anisotropic", anisotropic);
        material.insert("sheen", sheen);
        material.insert("sheenTint", sheenTint);
        material.insert("clearcoat", clearcoat);
        material.insert("clearcoatGloss", clearcoatGloss);
        return material;
    }

    void prase(const QJsonObject& ModelMaterial) {
        QJsonObject emissive = ModelMaterial["emissive"].toObject();
        QJsonObject baseColor = ModelMaterial["baseColor"].toObject();
        this->emissive = QVector3D(emissive["x"].toVariant().toFloat(), emissive["y"].toVariant().toFloat(), emissive["z"].toVariant().toFloat());
        this->baseColor = QVector3D(baseColor["r"].toVariant().toFloat(), baseColor["g"].toVariant().toFloat(), baseColor["b"].toVariant().toFloat());
        subsurface = ModelMaterial["subsurface"].toVariant().toFloat();
        metallic = ModelMaterial["metallic"].toVariant().toFloat();
        specular = ModelMaterial["specular"].toVariant().toFloat();
        specularTint = ModelMaterial["specularTint"].toVariant().toFloat();
        roughness = ModelMaterial["roughness"].toVariant().toFloat();
        anisotropic = ModelMaterial["anisotropic"].toVariant().toFloat();
        sheen = ModelMaterial["sheen"].toVariant().toFloat();
        sheenTint = ModelMaterial["sheenTint"].toVariant().toFloat();
        clearcoat = ModelMaterial["clearcoat"].toVariant().toFloat();
        clearcoatGloss = ModelMaterial["clearcoatGloss"].toVariant().toFloat();
    }

    MaterialEncoded encode() const {
        MaterialEncoded res;
        res.emissive = emissive;
        res.baseColor = baseColor;
        res.param1 = { subsurface, metallic, specular };
        res.param2 = { specularTint, roughness, anisotropic };
        res.param3 = { sheen, sheenTint, clearcoat };
        res.param4 = { clearcoatGloss, 0,0};
        res.param5 = { 0, 0, 0 };
        return res;
    }
};

struct LightMaterial
{
    QVector3D emissive;
    QVector3D baseColor;

    LightMaterial() {
        emissive = QVector3D(20, 20, 20);
        baseColor = QVector3D(1, 1, 1); //基本颜色
    }

    QJsonObject toJson() {
        QJsonObject material;
        QJsonObject emissive;
        QJsonObject baseColor;
        emissive.insert("x", this->emissive.x());
        emissive.insert("y", this->emissive.y());
        emissive.insert("z", this->emissive.z());
        material.insert("emissive", emissive);
        baseColor.insert("r", this->baseColor.x());
        baseColor.insert("g", this->baseColor.y());
        baseColor.insert("b", this->baseColor.z());
        material.insert("baseColor", baseColor);
        return material;
    }

    void prase(const QJsonObject& LightMaterial) {
        QJsonObject emissive = LightMaterial["emissive"].toObject();
        QJsonObject baseColor = LightMaterial["baseColor"].toObject();
        this->emissive = QVector3D(emissive["x"].toVariant().toFloat(), emissive["y"].toVariant().toFloat(), emissive["z"].toVariant().toFloat());
        this->baseColor = QVector3D(baseColor["r"].toVariant().toFloat(), baseColor["g"].toVariant().toFloat(), baseColor["b"].toVariant().toFloat());
    }

    ModelMaterial toModelMaterial() {
        ModelMaterial res;
        res.emissive = emissive;
        res.baseColor = baseColor;
        return res;
    }
};

#endif
