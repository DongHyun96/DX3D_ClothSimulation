#pragma once

class Snow : public Particle
{
private:
    struct VertexParticle
    {
        Vector3 pos = {};
        Vector2 size = {};
        Vector2 random = {};
    };

public:
    Snow(wstring file);
    ~Snow();

    virtual void Update() override;
    virtual void Render() override;
    virtual void Debug()  override;

private:
    virtual void SetVertex() override;

private:
    vector<VertexParticle> vertices;

    WeatherBuffer* buffer;
};