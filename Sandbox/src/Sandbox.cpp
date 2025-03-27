#include "Zenith.hpp"
#include "Zenith/EntryPoint.hpp"

#include "Zenith/ImGui/ImGuiLayer.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>

#include <string>

static void ImGuiShowHelpMarker(const char* desc)
{
  ImGui::TextDisabled("(?)");
  if (ImGui::IsItemHovered())
  {
    ImGui::BeginTooltip();
    ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
    ImGui::TextUnformatted(desc);
    ImGui::PopTextWrapPos();
    ImGui::EndTooltip();
  }
}

class TestLayer : public Zenith::Layer
{
public:
  TestLayer()
    : m_Scene(Scene::Spheres), m_Camera(glm::perspectiveFov(glm::radians(45.0f), 1280.0f, 720.0f, 0.1f, 10000.0f))
  {}

  virtual ~TestLayer()
  {}

  virtual void OnAttach() override
  {
    m_SimplePBRShader.reset(Zenith::Shader::Create("Resources/Shaders/simplepbr.glsl"));
    m_QuadShader.reset(Zenith::Shader::Create("Resources/Shaders/quad.glsl"));
    m_HDRShader.reset(Zenith::Shader::Create("Resources/Shaders/hdr.glsl"));
    m_Mesh.reset(new Zenith::Mesh("Resources/Meshes/cerberus.fbx"));
    m_SphereMesh.reset(new Zenith::Mesh("Resources/Models/Sphere.fbx"));

    // Editor
    m_CheckerboardTex.reset(Zenith::Texture2D::Create("Resources/Editor/Checkerboard.tga"));

    // Environment
    m_EnvironmentCubeMap.reset(Zenith::TextureCube::Create("Resources/Textures/environments/Arches_E_PineTree_Radiance.tga"));
    m_EnvironmentIrradiance.reset(Zenith::TextureCube::Create("Resources/Textures/environments/Arches_E_PineTree_Irradiance.tga"));
    m_BRDFLUT.reset(Zenith::Texture2D::Create("Resources/Textures/BRDF_LUT.tga"));

    m_Framebuffer.reset(Zenith::Framebuffer::Create(1280, 720, Zenith::FramebufferFormat::RGBA16F));
    m_FinalPresentBuffer.reset(Zenith::Framebuffer::Create(1280, 720, Zenith::FramebufferFormat::RGBA8));

    // Create Quad
    float x = -1, y = -1;
    float width = 2, height = 2;
    struct QuadVertex
    {
      glm::vec3 Position;
      glm::vec2 TexCoord;
    };

    QuadVertex* data = new QuadVertex[4];

    data[0].Position = glm::vec3(x, y, 0);
    data[0].TexCoord = glm::vec2(0, 0);

    data[1].Position = glm::vec3(x + width, y, 0);
    data[1].TexCoord = glm::vec2(1, 0);

    data[2].Position = glm::vec3(x + width, y + height, 0);
    data[2].TexCoord = glm::vec2(1, 1);

    data[3].Position = glm::vec3(x, y + height, 0);
    data[3].TexCoord = glm::vec2(0, 1);

    m_VertexBuffer.reset(Zenith::VertexBuffer::Create());
    m_VertexBuffer->SetData(data, 4 * sizeof(QuadVertex));

    uint32_t* indices = new uint32_t[6]{ 0, 1, 2, 2, 3, 0, };
    m_IndexBuffer.reset(Zenith::IndexBuffer::Create());
    m_IndexBuffer->SetData(indices, 6 * sizeof(uint32_t));

    m_Light.Direction = { -0.5f, -0.5f, 1.0f };
    m_Light.Radiance = { 1.0f, 1.0f, 1.0f };
  }

  virtual void OnDetach() override
  {}

  virtual void OnUpdate() override
  {
    // THINGS TO LOOK AT:
    // - BRDF LUT
    // - Cubemap mips and filtering
    // - Tonemapping and proper HDR pipeline
    using namespace Zenith;
    using namespace glm;

    m_Camera.Update();
    auto viewProjection = m_Camera.GetProjectionMatrix() * m_Camera.GetViewMatrix();

    m_Framebuffer->Bind();
    Renderer::Clear();

    Zenith::UniformBufferDeclaration<sizeof(mat4), 1> quadShaderUB;
    quadShaderUB.Push("u_InverseVP", inverse(viewProjection));
    m_QuadShader->UploadUniformBuffer(quadShaderUB);

    m_QuadShader->Bind();
    m_EnvironmentIrradiance->Bind(0);
    m_VertexBuffer->Bind();
    m_IndexBuffer->Bind();
    Renderer::DrawIndexed(m_IndexBuffer->GetCount(), false);

    Zenith::UniformBufferDeclaration<sizeof(mat4) * 2 + sizeof(vec3) * 4 + sizeof(float) * 8, 14> simplePbrShaderUB;
    simplePbrShaderUB.Push("u_ViewProjectionMatrix", viewProjection);
    simplePbrShaderUB.Push("u_ModelMatrix", mat4(1.0f));
    simplePbrShaderUB.Push("u_AlbedoColor", m_AlbedoInput.Color);
    simplePbrShaderUB.Push("u_Metalness", m_MetalnessInput.Value);
    simplePbrShaderUB.Push("u_Roughness", m_RoughnessInput.Value);
    simplePbrShaderUB.Push("lights.Direction", m_Light.Direction);
    simplePbrShaderUB.Push("lights.Radiance", m_Light.Radiance * m_LightMultiplier);
    simplePbrShaderUB.Push("u_CameraPosition", m_Camera.GetPosition());
    simplePbrShaderUB.Push("u_RadiancePrefilter", m_RadiancePrefilter ? 1.0f : 0.0f);
    simplePbrShaderUB.Push("u_AlbedoTexToggle", m_AlbedoInput.UseTexture ? 1.0f : 0.0f);
    simplePbrShaderUB.Push("u_NormalTexToggle", m_NormalInput.UseTexture ? 1.0f : 0.0f);
    simplePbrShaderUB.Push("u_MetalnessTexToggle", m_MetalnessInput.UseTexture ? 1.0f : 0.0f);
    simplePbrShaderUB.Push("u_RoughnessTexToggle", m_RoughnessInput.UseTexture ? 1.0f : 0.0f);
    simplePbrShaderUB.Push("u_EnvMapRotation", m_EnvMapRotation);
    m_SimplePBRShader->UploadUniformBuffer(simplePbrShaderUB);

    m_EnvironmentCubeMap->Bind(10);
    m_EnvironmentIrradiance->Bind(11);
    m_BRDFLUT->Bind(15);

    m_SimplePBRShader->Bind();
    if (m_AlbedoInput.TextureMap)
      m_AlbedoInput.TextureMap->Bind(1);
    if (m_NormalInput.TextureMap)
      m_NormalInput.TextureMap->Bind(2);
    if (m_MetalnessInput.TextureMap)
      m_MetalnessInput.TextureMap->Bind(3);
    if (m_RoughnessInput.TextureMap)
      m_RoughnessInput.TextureMap->Bind(4);

    if (m_Scene == Scene::Spheres)
    {
      // Metals
      float roughness = 0.0f;
      float x = -88.0f;
      for (int i = 0; i < 8; i++)
      {
        m_SimplePBRShader->SetMat4("u_ModelMatrix", translate(mat4(1.0f), vec3(x, 0.0f, 0.0f)));
        m_SimplePBRShader->SetFloat("u_Roughness", roughness);
        m_SimplePBRShader->SetFloat("u_Metalness", 1.0f);
        m_SphereMesh->Render();

        roughness += 0.15f;
        x += 22.0f;
      }

      // Dielectrics
      roughness = 0.0f;
      x = -88.0f;
      for (int i = 0; i < 8; i++)
      {
        m_SimplePBRShader->SetMat4("u_ModelMatrix", translate(mat4(1.0f), vec3(x, 22.0f, 0.0f)));
        m_SimplePBRShader->SetFloat("u_Roughness", roughness);
        m_SimplePBRShader->SetFloat("u_Metalness", 0.0f);
        m_SphereMesh->Render();

        roughness += 0.15f;
        x += 22.0f;
      }

    }
    else if (m_Scene == Scene::Model)
    {
      m_Mesh->Render();
    }

    m_Framebuffer->Unbind();

    m_FinalPresentBuffer->Bind();
    m_HDRShader->Bind();
    m_HDRShader->SetFloat("u_Exposure", m_Exposure);
    m_Framebuffer->BindTexture();
    m_VertexBuffer->Bind();
    m_IndexBuffer->Bind();
    Renderer::DrawIndexed(m_IndexBuffer->GetCount(), false);
    m_FinalPresentBuffer->Unbind();
  }

  virtual void OnImGuiRender() override
  {}

  virtual void OnEvent(Zenith::Event& event) override
  {}
private:
  std::unique_ptr<Zenith::Shader> m_Shader;
  std::unique_ptr<Zenith::Shader> m_PBRShader;
  std::unique_ptr<Zenith::Shader> m_SimplePBRShader;
  std::unique_ptr<Zenith::Shader> m_QuadShader;
  std::unique_ptr<Zenith::Shader> m_HDRShader;
  std::unique_ptr<Zenith::Mesh> m_Mesh;
  std::unique_ptr<Zenith::Mesh> m_SphereMesh;
  std::unique_ptr<Zenith::Texture2D> m_BRDFLUT;

  struct AlbedoInput
  {
    glm::vec3 Color = { 0.972f, 0.96f, 0.915f }; // Silver, from https://docs.unrealengine.com/en-us/Engine/Rendering/Materials/PhysicallyBased
    std::unique_ptr<Zenith::Texture2D> TextureMap;
    bool SRGB = true;
    bool UseTexture = false;
  };
  AlbedoInput m_AlbedoInput;

  struct NormalInput
  {
    std::unique_ptr<Zenith::Texture2D> TextureMap;
    bool UseTexture = false;
  };
  NormalInput m_NormalInput;

  struct MetalnessInput
  {
    float Value = 1.0f;
    std::unique_ptr<Zenith::Texture2D> TextureMap;
    bool UseTexture = false;
  };
  MetalnessInput m_MetalnessInput;

  struct RoughnessInput
  {
    float Value = 0.5f;
    std::unique_ptr<Zenith::Texture2D> TextureMap;
    bool UseTexture = false;
  };
  RoughnessInput m_RoughnessInput;

  std::unique_ptr<Zenith::Framebuffer> m_Framebuffer, m_FinalPresentBuffer;

  std::unique_ptr<Zenith::VertexBuffer> m_VertexBuffer;
  std::unique_ptr<Zenith::IndexBuffer> m_IndexBuffer;
  std::unique_ptr<Zenith::TextureCube> m_EnvironmentCubeMap, m_EnvironmentIrradiance;

  Zenith::Camera m_Camera;

  struct Light
  {
    glm::vec3 Direction;
    glm::vec3 Radiance;
  };
  Light m_Light;
  float m_LightMultiplier = 0.3f;

  // PBR params
  float m_Exposure = 1.0f;

  bool m_RadiancePrefilter = false;

  float m_EnvMapRotation = 0.0f;

  enum class Scene : uint32_t
  {
    Spheres = 0, Model = 1
  };
  Scene m_Scene;

  // Editor resources
  std::unique_ptr<Zenith::Texture2D> m_CheckerboardTex;
};

class Sandbox : public Zenith::Application
{
public:
  Sandbox()
  {
    ZN_TRACE("Hello!");
  }

  virtual void OnInit() override
  {
    PushLayer(new TestLayer());
  }
};

Zenith::Application* Zenith::CreateApplication()
{
  return new Sandbox();
}
