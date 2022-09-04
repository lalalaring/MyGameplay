The `gameplay::Model` class is the basic component used to draw geometry in your scene. The model contains a few key elements: a `gameplay::Mesh`, an optional `gameplay::MeshSkin` and one or more `gameplay::Material`. These contribute to the information that is needed to perform the rendering of a model.

## Mesh geometry

The `gameplay::Mesh` class consists of a `gameplay::VertexFormat` attribute. This attribute describes the layout for the vertex data as well as the actual vertex data, which is used as input in the rendering of the geometry. In addition, it holds one or more `gameplay::MeshParts`. These parts define the primitive shapes and indices into the vertex data that describe how the vertices are connected.

Game artists use 3-D modeling tools that are capable of organizing and splitting the vertex data into parts/subsets based on the materials that are applied to them. The `gameplay::Mesh` class maintains one vertex buffer to hold all the vertices, and for each `gameplay::MeshPart`, an index buffer is used to draw the primitive shapes.

## MeshSkin and Joints

The `gameplay::Mesh` class supports an optional `gameplay::MeshSkin`. This is used when loading models that represent characters in the game that have a skeleton consisting of `gameplay::Joint` objects (bones). 
Vertex skinning is the term used to describe the process of applying a weighting or relationship with the Joints and their affected vertices. 
Using 3-D modeling tools, artists can add this additional weighting information onto the vertices in order to control how much a particular vertex should be impacted. 
This is based on the transformation of joints that can affect them. You will learn later how to apply special, skinned Materials that support this weighting. 
The gameplay 3-D framework supports a maximum of four blend weights per vertex. The `gameplay::MeshSkin` class holds and maintains a hierarchy of `gameplay::Joint` objects that can be transformed. 
A typical operation is to animate the transformation (usually only rotations) of the joints. The data within this class can be bound onto skinned Materials to ensure proper impact of weights onto their influenced vertices.