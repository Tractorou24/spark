#pragma pack_matrix(row_major)

struct VertexData
{
    float4 Position : SV_POSITION;
    float4 Color : COLOR;
    float3 Circle : TEXCOORD0; // (x, y, radius)
};

struct FragmentData
{
    float4 Color : SV_TARGET;
    float Depth : SV_DEPTH;
};

FragmentData main(VertexData input)
{
    // If the shape to render is a circle (radius > 0), discard the fragment if it's outside the circle
    if(input.Circle.z > 0)
        if (length(input.Position.xy - input.Circle.xy) > input.Circle.z)
            discard;

    FragmentData fragment;
    fragment.Depth = input.Position.z;
    fragment.Color = input.Color;
    return fragment;
}
