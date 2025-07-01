#version 330 core

uniform uint object_id; 

out uint fragColor;

void main()
{
    fragColor = object_id;
}
