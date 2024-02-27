#pragma pack_matrix(row_major)

struct VertexData
{
    float4 Position : SV_POSITION;
    float4 Color : COLOR;
    float3 Circle : TEXCOORD0; // (x, y, radius)
};

struct VertexInput
{
    //[[vk::location(0)]] 
    float3 Position : POSITION;
};

struct CameraData
{
    float4x4 ViewProjection;
};

struct InstanceData
{
    float4x4 Transform;
    float4 Color;
    float Circle;
};

StructuredBuffer<InstanceData> instances[] : register(t0, space0);
[[vk::push_constant]] ConstantBuffer<CameraData> camera : register(b0, space1);

VertexData main(in VertexInput input, uint id : SV_InstanceID)
{
    VertexData vertex;      
    InstanceData instance = instances[NonUniformResourceIndex(id)].Load(0);

    float4 position = mul(float4(input.Position, 1.0), instance.Transform);
    vertex.Position = mul(position, camera.ViewProjection);
    vertex.Color = instance.Color;
    vertex.Circle = float3(instance.Transform._41_42, instance.Circle);
    return vertex;
}
