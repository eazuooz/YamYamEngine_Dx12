


struct VS_IN
{
	float3 pos : POSITION;
	float4 color : COLOR;
};

struct VS_OUT
{
    float pos : SV_Position;
    float color : COLOR;
};

VS_OUT VS_Main(VS_IN input)
{
    VS_OUT output = (VS_OUT) 0;
    output.pos = input.pos;
    output.color = input.color;
    
    return output;
}

float4 PS_Main(VS_OUT input) : SV_Target
{
    return input.color;
}