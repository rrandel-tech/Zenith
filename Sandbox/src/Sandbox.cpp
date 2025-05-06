#include "Zenith.hpp"
#include "Zenith/EntryPoint.hpp"

#include "Zenith/ImGui/ImGuizmo.h"
#include "Zenith/ImGui/ImGuiLayer.hpp"
#include "imgui/imgui_internal.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include <string>
#include <Zenith/Core/KeyCodes.hpp>

#include "Zenith/Editor/SceneHierarchyPanel.hpp"
#include "Zenith/Renderer/Renderer2D.hpp"

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

class EditorLayer : public Zenith::Layer
{
public:
	EditorLayer()
		: m_SceneType(SceneType::Model)
	{}

	virtual ~EditorLayer()
	{}

	virtual void OnAttach() override
	{
		// ImGui Colors
		ImVec4* colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
		colors[ImGuiCol_TextDisabled] = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
		colors[ImGuiCol_WindowBg] = ImVec4(0.18f, 0.18f, 0.18f, 1.0f); // Window background
		colors[ImGuiCol_ChildBg] = ImVec4(1.0f, 1.0f, 1.0f, 0.0f);
		colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
		colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.50f, 0.5f);
		colors[ImGuiCol_BorderShadow] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
		colors[ImGuiCol_FrameBg] = ImVec4(0.3f, 0.3f, 0.3f, 0.5f); // Widget backgrounds
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.4f, 0.4f, 0.4f, 0.4f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.4f, 0.4f, 0.4f, 0.6f);
		colors[ImGuiCol_TitleBg] = ImVec4(0.04f, 0.04f, 0.04f, 1.0f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.29f, 0.29f, 0.29f, 1.0f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.0f, 0.0f, 0.0f, 0.51f);
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.0f);
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.0f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.0f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.0f);
		colors[ImGuiCol_CheckMark] = ImVec4(0.94f, 0.94f, 0.94f, 1.0f);
		colors[ImGuiCol_SliderGrab] = ImVec4(0.51f, 0.51f, 0.51f, 0.7f);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(0.66f, 0.66f, 0.66f, 1.0f);
		colors[ImGuiCol_Button] = ImVec4(0.44f, 0.44f, 0.44f, 0.4f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.46f, 0.47f, 0.48f, 1.0f);
		colors[ImGuiCol_ButtonActive] = ImVec4(0.42f, 0.42f, 0.42f, 1.0f);
		colors[ImGuiCol_Header] = ImVec4(0.7f, 0.7f, 0.7f, 0.31f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.7f, 0.7f, 0.7f, 0.8f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.48f, 0.5f, 0.52f, 1.0f);
		colors[ImGuiCol_Separator] = ImVec4(0.43f, 0.43f, 0.5f, 0.5f);
		colors[ImGuiCol_SeparatorHovered] = ImVec4(0.72f, 0.72f, 0.72f, 0.78f);
		colors[ImGuiCol_SeparatorActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.0f);
		colors[ImGuiCol_ResizeGrip] = ImVec4(0.91f, 0.91f, 0.91f, 0.25f);
		colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.81f, 0.81f, 0.81f, 0.67f);
		colors[ImGuiCol_ResizeGripActive] = ImVec4(0.46f, 0.46f, 0.46f, 0.95f);
		colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.0f);
		colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.0f, 0.43f, 0.35f, 1.0f);
		colors[ImGuiCol_PlotHistogram] = ImVec4(0.73f, 0.6f, 0.15f, 1.0f);
		colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.0f, 0.6f, 0.0f, 1.0f);
		colors[ImGuiCol_TextSelectedBg] = ImVec4(0.87f, 0.87f, 0.87f, 0.35f);
		colors[ImGuiCol_DragDropTarget] = ImVec4(1.0f, 1.0f, 0.0f, 0.9f);
		colors[ImGuiCol_NavHighlight] = ImVec4(0.60f, 0.6f, 0.6f, 1.0f);
		colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.0f, 1.0f, 1.0f, 0.7f);

		using namespace glm;

		auto environment = Zenith::Environment::Load("Resources/Env/birchwood_4k.hdr");

		// Model Scene
		{
			m_Scene = Zenith::CreateRef<Zenith::Scene>("Model Scene");
			m_Scene->SetCamera(Zenith::Camera(glm::perspectiveFov(glm::radians(45.0f), 1280.0f, 720.0f, 0.1f, 10000.0f)));

			m_Scene->SetEnvironment(environment);

			m_MeshEntity = m_Scene->CreateEntity("Test Entity");

			auto mesh = Zenith::CreateRef<Zenith::Mesh>("Resources/Meshes/TestScene.fbx");
			m_MeshEntity->SetMesh(mesh);

			m_MeshMaterial = mesh->GetMaterial();

			auto secondEntity = m_Scene->CreateEntity("Gun Entity");
			secondEntity->Transform() = glm::translate(glm::mat4(1.0f), { 5, 5, 5 }) * glm::scale(glm::mat4(1.0f), { 10, 10, 10 });
			mesh = Zenith::CreateRef<Zenith::Mesh>("Resources/Models/m1911/m1911.fbx");
			secondEntity->SetMesh(mesh);
		}

		// Sphere Scene
		{
			m_SphereScene = Zenith::CreateRef<Zenith::Scene>("PBR Sphere Scene");
			m_SphereScene->SetCamera(Zenith::Camera(glm::perspectiveFov(glm::radians(45.0f), 1280.0f, 720.0f, 0.1f, 10000.0f)));

			m_SphereScene->SetEnvironment(environment);

			auto sphereMesh = Zenith::CreateRef<Zenith::Mesh>("Resources/Models/Sphere1m.fbx");
			m_SphereBaseMaterial = sphereMesh->GetMaterial();

			float x = -4.0f;
			float roughness = 0.0f;
			for (int i = 0; i < 8; i++)
			{
				auto sphereEntity = m_SphereScene->CreateEntity();

				Zenith::Ref<Zenith::MaterialInstance> mi = CreateRef<Zenith::MaterialInstance>(m_SphereBaseMaterial);
				mi->Set("u_Metalness", 1.0f);
				mi->Set("u_Roughness", roughness);
				x += 1.1f;
				roughness += 0.15f;
				m_MetalSphereMaterialInstances.push_back(mi);

				sphereEntity->SetMesh(sphereMesh);
				sphereEntity->SetMaterial(mi);
				sphereEntity->Transform() = translate(mat4(1.0f), vec3(x, 0.0f, 0.0f));
			}

			x = -4.0f;
			roughness = 0.0f;
			for (int i = 0; i < 8; i++)
			{
				auto sphereEntity = m_SphereScene->CreateEntity();

				Zenith::Ref<Zenith::MaterialInstance> mi(new Zenith::MaterialInstance(m_SphereBaseMaterial));
				mi->Set("u_Metalness", 0.0f);
				mi->Set("u_Roughness", roughness);
				x += 1.1f;
				roughness += 0.15f;
				m_DielectricSphereMaterialInstances.push_back(mi);

				sphereEntity->SetMesh(sphereMesh);
				sphereEntity->SetMaterial(mi);
				sphereEntity->Transform() = translate(mat4(1.0f), vec3(x, 1.2f, 0.0f));
			}
		}

		m_ActiveScene = m_Scene;
		m_SceneHierarchyPanel = CreateScope<Zenith::SceneHierarchyPanel>(m_ActiveScene);

		m_PlaneMesh.reset(new Zenith::Mesh("Resources/Models/Plane1m.obj"));

		// Editor
		m_CheckerboardTex = Zenith::Texture2D::Create("Resources/Editor/Checkerboard.tga");

		// Set lights

		m_Light.Direction = { -0.5f, -0.5f, 1.0f };
		m_Light.Radiance = { 1.0f, 1.0f, 1.0f };
	}

	virtual void OnDetach() override
	{}

	virtual void OnUpdate(Zenith::Timestep ts) override
	{
		// THINGS TO LOOK AT:
		// - BRDF LUT
		// - Tonemapping and proper HDR pipeline
		using namespace Zenith;
		using namespace glm;

		m_MeshMaterial->Set("u_AlbedoColor", m_AlbedoInput.Color);
		m_MeshMaterial->Set("u_Metalness", m_MetalnessInput.Value);
		m_MeshMaterial->Set("u_Roughness", m_RoughnessInput.Value);
		m_MeshMaterial->Set("lights", m_Light);
		m_MeshMaterial->Set("u_AlbedoTexToggle", m_AlbedoInput.UseTexture ? 1.0f : 0.0f);
		m_MeshMaterial->Set("u_NormalTexToggle", m_NormalInput.UseTexture ? 1.0f : 0.0f);
		m_MeshMaterial->Set("u_MetalnessTexToggle", m_MetalnessInput.UseTexture ? 1.0f : 0.0f);
		m_MeshMaterial->Set("u_RoughnessTexToggle", m_RoughnessInput.UseTexture ? 1.0f : 0.0f);
		m_MeshMaterial->Set("u_EnvMapRotation", m_EnvMapRotation);

		m_SphereBaseMaterial->Set("u_AlbedoColor", m_AlbedoInput.Color);
		m_SphereBaseMaterial->Set("lights", m_Light);
		m_SphereBaseMaterial->Set("u_RadiancePrefilter", m_RadiancePrefilter ? 1.0f : 0.0f);
		m_SphereBaseMaterial->Set("u_AlbedoTexToggle", m_AlbedoInput.UseTexture ? 1.0f : 0.0f);
		m_SphereBaseMaterial->Set("u_NormalTexToggle", m_NormalInput.UseTexture ? 1.0f : 0.0f);
		m_SphereBaseMaterial->Set("u_MetalnessTexToggle", m_MetalnessInput.UseTexture ? 1.0f : 0.0f);
		m_SphereBaseMaterial->Set("u_RoughnessTexToggle", m_RoughnessInput.UseTexture ? 1.0f : 0.0f);
		m_SphereBaseMaterial->Set("u_EnvMapRotation", m_EnvMapRotation);

		if (m_AlbedoInput.TextureMap)
			m_MeshMaterial->Set("u_AlbedoTexture", m_AlbedoInput.TextureMap);
		if (m_NormalInput.TextureMap)
			m_MeshMaterial->Set("u_NormalTexture", m_NormalInput.TextureMap);
		if (m_MetalnessInput.TextureMap)
			m_MeshMaterial->Set("u_MetalnessTexture", m_MetalnessInput.TextureMap);
		if (m_RoughnessInput.TextureMap)
			m_MeshMaterial->Set("u_RoughnessTexture", m_RoughnessInput.TextureMap);

		if (m_AllowViewportCameraEvents)
			m_Scene->GetCamera().OnUpdate(ts);

		m_ActiveScene->OnUpdate(ts);

		if (m_DrawOnTopBoundingBoxes)
		{
			Zenith::Renderer::BeginRenderPass(Zenith::SceneRenderer::GetFinalRenderPass(), false);
			auto viewProj = m_Scene->GetCamera().GetViewProjection();
			Zenith::Renderer2D::BeginScene(viewProj, false);
			// Zenith::Renderer2D::DrawQuad({ 0, 0, 0 }, { 4.0f, 5.0f }, { 1.0f, 1.0f, 0.5f, 1.0f });
			Renderer::DrawAABB(m_MeshEntity->GetMesh());
			Zenith::Renderer2D::EndScene();
			Zenith::Renderer::EndRenderPass();
		}
	}

	enum class PropertyFlag
	{
		None = 0, ColorProperty = 1
	};

	bool Property(const std::string& name, bool& value)
	{
		ImGui::Text(name.c_str());
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);

		std::string id = "##" + name;
		bool result = ImGui::Checkbox(id.c_str(), &value);

		ImGui::PopItemWidth();
		ImGui::NextColumn();

		return result;
	}

	void Property(const std::string& name, float& value, float min = -1.0f, float max = 1.0f, PropertyFlag flags = PropertyFlag::None)
	{
		ImGui::Text(name.c_str());
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);

		std::string id = "##" + name;
		ImGui::SliderFloat(id.c_str(), &value, min, max);

		ImGui::PopItemWidth();
		ImGui::NextColumn();
	}

	void Property(const std::string& name, glm::vec2& value, PropertyFlag flags)
	{
		Property(name, value, -1.0f, 1.0f, flags);
	}

	void Property(const std::string& name, glm::vec2& value, float min, float max, PropertyFlag flags)
	{
		ImGui::Text(name.c_str());
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);

		std::string id = "##" + name;
		ImGui::SliderFloat2(id.c_str(), glm::value_ptr(value), min, max);

		ImGui::PopItemWidth();
		ImGui::NextColumn();
	}

	void Property(const std::string& name, glm::vec3& value, PropertyFlag flags)
	{
		Property(name, value, -1.0f, 1.0f, flags);
	}

	void Property(const std::string& name, glm::vec3& value, float min = -1.0f, float max = 1.0f, PropertyFlag flags = PropertyFlag::None)
	{
		ImGui::Text(name.c_str());
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);

		std::string id = "##" + name;
		if ((int)flags & (int)PropertyFlag::ColorProperty)
			ImGui::ColorEdit3(id.c_str(), glm::value_ptr(value), ImGuiColorEditFlags_NoInputs);
		else
			ImGui::SliderFloat3(id.c_str(), glm::value_ptr(value), min, max);

		ImGui::PopItemWidth();
		ImGui::NextColumn();
	}

	void Property(const std::string& name, glm::vec4& value, PropertyFlag flags)
	{
		Property(name, value, -1.0f, 1.0f, flags);
	}

	void Property(const std::string& name, glm::vec4& value, float min = -1.0f, float max = 1.0f, PropertyFlag flags = PropertyFlag::None)
	{
		ImGui::Text(name.c_str());
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);

		std::string id = "##" + name;
		if ((int)flags & (int)PropertyFlag::ColorProperty)
			ImGui::ColorEdit4(id.c_str(), glm::value_ptr(value), ImGuiColorEditFlags_NoInputs);
		else
			ImGui::SliderFloat4(id.c_str(), glm::value_ptr(value), min, max);

		ImGui::PopItemWidth();
		ImGui::NextColumn();
	}

	void ShowBoundingBoxes(bool show, bool onTop)
	{
		Zenith::SceneRenderer::GetOptions().ShowBoundingBoxes = show && !onTop;
		m_DrawOnTopBoundingBoxes = show && onTop;
	}

	virtual void OnImGuiRender() override
	{
		static bool p_open = true;

		static bool opt_fullscreen_persistant = true;
		static ImGuiDockNodeFlags opt_flags = ImGuiDockNodeFlags_None;
		bool opt_fullscreen = opt_fullscreen_persistant;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &p_open, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// Dockspace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), opt_flags);
		}

		// Editor Panel ------------------------------------------------------------------------------
		ImGui::Begin("Model");

		if (ImGui::RadioButton("Spheres", (int*)&m_SceneType, (int)SceneType::Spheres))
			m_ActiveScene = m_SphereScene;
		ImGui::SameLine();
		if (ImGui::RadioButton("Model", (int*)&m_SceneType, (int)SceneType::Model))
			m_ActiveScene = m_Scene;

		ImGui::Begin("Environment");

		if (ImGui::Button("Load Environment Map"))
		{
			std::string filename = Zenith::Application::Get().OpenFile("*.hdr");
			if (filename != "")
				m_ActiveScene->SetEnvironment(Zenith::Environment::Load(filename));
		}

		ImGui::SliderFloat("Skybox LOD", &m_Scene->GetSkyboxLod(), 0.0f, 11.0f);

		ImGui::Columns(2);
		ImGui::AlignTextToFramePadding();

		Property("Light Direction", m_Light.Direction);
		Property("Light Radiance", m_Light.Radiance, PropertyFlag::ColorProperty);
		Property("Light Multiplier", m_LightMultiplier, 0.0f, 5.0f);
		Property("Exposure", m_ActiveScene->GetCamera().GetExposure(), 0.0f, 5.0f);

		Property("Radiance Prefiltering", m_RadiancePrefilter);
		Property("Env Map Rotation", m_EnvMapRotation, -360.0f, 360.0f);

		if (Property("Show Bounding Boxes", m_UIShowBoundingBoxes))
			ShowBoundingBoxes(m_UIShowBoundingBoxes, m_UIShowBoundingBoxesOnTop);
		if (m_UIShowBoundingBoxes && Property("On Top", m_UIShowBoundingBoxesOnTop))
			ShowBoundingBoxes(m_UIShowBoundingBoxes, m_UIShowBoundingBoxesOnTop);

		ImGui::Columns(1);

		ImGui::End();

		ImGui::Separator();
		{
			ImGui::Text("Mesh");
			auto mesh = m_MeshEntity->GetMesh();
			std::string fullpath = mesh ? mesh->GetFilePath() : "None";
			size_t found = fullpath.find_last_of("/\\");
			std::string path = found != std::string::npos ? fullpath.substr(found + 1) : fullpath;
			ImGui::Text(path.c_str()); ImGui::SameLine();
			if (ImGui::Button("...##Mesh"))
			{
				std::string filename = Zenith::Application::Get().OpenFile("");
				if (filename != "")
				{
					auto newMesh = Zenith::CreateRef<Zenith::Mesh>(filename);
					// m_MeshMaterial.reset(new MaterialInstance(newMesh->GetMaterial()));
					// m_MeshEntity->SetMaterial(m_MeshMaterial);
					m_MeshEntity->SetMesh(newMesh);
				}
			}
		}
		ImGui::Separator();

		// Textures ------------------------------------------------------------------------------
		{
			// Albedo
			if (ImGui::CollapsingHeader("Albedo", nullptr, ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 10));
				ImGui::Image((ImTextureID)(m_AlbedoInput.TextureMap ? (void*)m_AlbedoInput.TextureMap->GetRendererID() : (void*)m_CheckerboardTex->GetRendererID()), ImVec2(64, 64));
				ImGui::PopStyleVar();
				if (ImGui::IsItemHovered())
				{
					if (m_AlbedoInput.TextureMap)
					{
						ImGui::BeginTooltip();
						ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
						ImGui::TextUnformatted(m_AlbedoInput.TextureMap->GetPath().c_str());
						ImGui::PopTextWrapPos();
						ImGui::Image((ImTextureID)(uintptr_t)m_AlbedoInput.TextureMap->GetRendererID(), ImVec2(384, 384));
						ImGui::EndTooltip();
					}
					if (ImGui::IsItemClicked())
					{
						std::string filename = Zenith::Application::Get().OpenFile("");
						if (filename != "")
							m_AlbedoInput.TextureMap = (Zenith::Texture2D::Create(filename, m_AlbedoInput.SRGB));
					}
				}
				ImGui::SameLine();
				ImGui::BeginGroup();
				ImGui::Checkbox("Use##AlbedoMap", &m_AlbedoInput.UseTexture);
				if (ImGui::Checkbox("sRGB##AlbedoMap", &m_AlbedoInput.SRGB))
				{
					if (m_AlbedoInput.TextureMap)
						m_AlbedoInput.TextureMap = (Zenith::Texture2D::Create(m_AlbedoInput.TextureMap->GetPath(), m_AlbedoInput.SRGB));
				}
				ImGui::EndGroup();
				ImGui::SameLine();
				ImGui::ColorEdit3("Color##Albedo", glm::value_ptr(m_AlbedoInput.Color), ImGuiColorEditFlags_NoInputs);
			}
		}
		{
			// Normals
			if (ImGui::CollapsingHeader("Normals", nullptr, ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 10));
				ImGui::Image((ImTextureID)(m_NormalInput.TextureMap ? (void*)m_NormalInput.TextureMap->GetRendererID() : (void*)m_CheckerboardTex->GetRendererID()), ImVec2(64, 64));
				ImGui::PopStyleVar();
				if (ImGui::IsItemHovered())
				{
					if (m_NormalInput.TextureMap)
					{
						ImGui::BeginTooltip();
						ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
						ImGui::TextUnformatted(m_NormalInput.TextureMap->GetPath().c_str());
						ImGui::PopTextWrapPos();
						ImGui::Image((ImTextureID)(uintptr_t)m_NormalInput.TextureMap->GetRendererID(), ImVec2(384, 384));
						ImGui::EndTooltip();
					}
					if (ImGui::IsItemClicked())
					{
						std::string filename = Zenith::Application::Get().OpenFile("");
						if (filename != "")
							m_NormalInput.TextureMap = (Zenith::Texture2D::Create(filename));
					}
				}
				ImGui::SameLine();
				ImGui::Checkbox("Use##NormalMap", &m_NormalInput.UseTexture);
			}
		}
		{
			// Metalness
			if (ImGui::CollapsingHeader("Metalness", nullptr, ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 10));
				ImGui::Image((ImTextureID)(m_MetalnessInput.TextureMap ? (void*)m_MetalnessInput.TextureMap->GetRendererID() : (void*)m_CheckerboardTex->GetRendererID()), ImVec2(64, 64));
				ImGui::PopStyleVar();
				if (ImGui::IsItemHovered())
				{
					if (m_MetalnessInput.TextureMap)
					{
						ImGui::BeginTooltip();
						ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
						ImGui::TextUnformatted(m_MetalnessInput.TextureMap->GetPath().c_str());
						ImGui::PopTextWrapPos();
						ImGui::Image((ImTextureID)(uintptr_t)m_MetalnessInput.TextureMap->GetRendererID(), ImVec2(384, 384));
						ImGui::EndTooltip();
					}
					if (ImGui::IsItemClicked())
					{
						std::string filename = Zenith::Application::Get().OpenFile("");
						if (filename != "")
							m_MetalnessInput.TextureMap = (Zenith::Texture2D::Create(filename));
					}
				}
				ImGui::SameLine();
				ImGui::Checkbox("Use##MetalnessMap", &m_MetalnessInput.UseTexture);
				ImGui::SameLine();
				ImGui::SliderFloat("Value##MetalnessInput", &m_MetalnessInput.Value, 0.0f, 1.0f);
			}
		}
		{
			// Roughness
			if (ImGui::CollapsingHeader("Roughness", nullptr, ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 10));
				ImGui::Image((ImTextureID)(m_RoughnessInput.TextureMap ? (void*)m_RoughnessInput.TextureMap->GetRendererID() : (void*)m_CheckerboardTex->GetRendererID()), ImVec2(64, 64));
				ImGui::PopStyleVar();
				if (ImGui::IsItemHovered())
				{
					if (m_RoughnessInput.TextureMap)
					{
						ImGui::BeginTooltip();
						ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
						ImGui::TextUnformatted(m_RoughnessInput.TextureMap->GetPath().c_str());
						ImGui::PopTextWrapPos();
						ImGui::Image((ImTextureID)(uintptr_t)m_RoughnessInput.TextureMap->GetRendererID(), ImVec2(384, 384));
						ImGui::EndTooltip();
					}
					if (ImGui::IsItemClicked())
					{
						std::string filename = Zenith::Application::Get().OpenFile("");
						if (filename != "")
							m_RoughnessInput.TextureMap = (Zenith::Texture2D::Create(filename));
					}
				}
				ImGui::SameLine();
				ImGui::Checkbox("Use##RoughnessMap", &m_RoughnessInput.UseTexture);
				ImGui::SameLine();
				ImGui::SliderFloat("Value##RoughnessInput", &m_RoughnessInput.Value, 0.0f, 1.0f);
			}
		}

		ImGui::Separator();

		if (ImGui::TreeNode("Shaders"))
		{
			auto& shaders = Zenith::Shader::s_AllShaders;
			for (auto& shader : shaders)
			{
				if (ImGui::TreeNode(shader->GetName().c_str()))
				{
					std::string buttonName = "Reload##" + shader->GetName();
					if (ImGui::Button(buttonName.c_str()))
						shader->Reload();
					ImGui::TreePop();
				}
			}
			ImGui::TreePop();
		}

		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::Begin("Viewport");

		auto viewportSize = ImGui::GetContentRegionAvail();
		Zenith::SceneRenderer::SetViewportSize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
		m_ActiveScene->GetCamera().SetProjectionMatrix(glm::perspectiveFov(glm::radians(45.0f), viewportSize.x, viewportSize.y, 0.1f, 10000.0f));
		m_ActiveScene->GetCamera().SetViewportSize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
		ImGui::Image((ImTextureID)(uintptr_t)(Zenith::SceneRenderer::GetFinalColorBufferRendererID()), viewportSize, { 0, 1 }, { 1, 0 });

		static int counter = 0;
		auto windowSize = ImGui::GetWindowSize();
		ImVec2 minBound = ImGui::GetWindowPos();
		ImVec2 maxBound = { minBound.x + windowSize.x, minBound.y + windowSize.y };
		m_AllowViewportCameraEvents = ImGui::IsMouseHoveringRect(minBound, maxBound);

		// Gizmos
		if (m_GizmoType != -1)
		{
			float rw = (float)ImGui::GetWindowWidth();
			float rh = (float)ImGui::GetWindowHeight();
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();
			ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, rw, rh);
			ImGuizmo::Manipulate(glm::value_ptr(m_ActiveScene->GetCamera().GetViewMatrix()), glm::value_ptr(m_ActiveScene->GetCamera().GetProjectionMatrix()), (ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(m_MeshEntity->Transform()));
		}

		ImGui::End();
		ImGui::PopStyleVar();

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Docking"))
			{
				// Disabling fullscreen would allow the window to be moved to the front of other windows, 
				// which we can't undo at the moment without finer window depth/z control.
				//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

				if (ImGui::MenuItem("Flag: NoSplit", "", (opt_flags & ImGuiDockNodeFlags_NoSplit) != 0))                 opt_flags ^= ImGuiDockNodeFlags_NoSplit;
				if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "", (opt_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0))  opt_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode;
				if (ImGui::MenuItem("Flag: NoResize", "", (opt_flags & ImGuiDockNodeFlags_NoResize) != 0))                opt_flags ^= ImGuiDockNodeFlags_NoResize;
				if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (opt_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0))          opt_flags ^= ImGuiDockNodeFlags_AutoHideTabBar;
				ImGui::Separator();
				if (ImGui::MenuItem("Close DockSpace", NULL, false, p_open != NULL))
					p_open = false;
				ImGui::EndMenu();
			}
			ImGuiShowHelpMarker(
				"You can _always_ dock _any_ window into another by holding the SHIFT key while moving a window. Try it now!" "\n"
				"This demo app has nothing to do with it!" "\n\n"
				"This demo app only demonstrate the use of ImGui::DockSpace() which allows you to manually create a docking node _within_ another window. This is useful so you can decorate your main application window (e.g. with a menu bar)." "\n\n"
				"ImGui::DockSpace() comes with one hard constraint: it needs to be submitted _before_ any window which may be docked into it. Therefore, if you use a dock spot as the central point of your application, you'll probably want it to be part of the very first window you are submitting to imgui every frame." "\n\n"
				"(NB: because of this constraint, the implicit \"Debug\" window can not be docked into an explicit DockSpace() node, because that window is submitted as part of the NewFrame() call. An easy workaround is that you can create your own implicit \"Debug##2\" window after calling DockSpace() and leave it in the window stack for anyone to use.)"
			);

			ImGui::EndMenuBar();
		}

		m_SceneHierarchyPanel->OnImGuiRender();

		// static bool o = true;
		// ImGui::ShowDemoWindow(&o);
		ImGui::End();
	}

	virtual void OnEvent(Zenith::Event& e) override
	{
		if (m_AllowViewportCameraEvents)
			m_Scene->GetCamera().OnEvent(e);

		Zenith::EventDispatcher dispatcher(e);
		dispatcher.Dispatch<Zenith::KeyPressedEvent>(ZN_BIND_EVENT_FN(EditorLayer::OnKeyPressedEvent));
	}

	bool OnKeyPressedEvent(Zenith::KeyPressedEvent& e)
	{
		switch (e.GetKeyCode())
		{
		case ZN_KEY_Q:
			m_GizmoType = -1;
			break;
		case ZN_KEY_W:
			m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
			break;
		case ZN_KEY_E:
			m_GizmoType = ImGuizmo::OPERATION::ROTATE;
			break;
		case ZN_KEY_R:
			m_GizmoType = ImGuizmo::OPERATION::SCALE;
			break;
		case ZN_KEY_G:
			// Toggle grid
			if (Zenith::Input::IsKeyPressed(ZN_KEY_LEFT_CONTROL))
				Zenith::SceneRenderer::GetOptions().ShowGrid = !Zenith::SceneRenderer::GetOptions().ShowGrid;
			break;
		case ZN_KEY_B:
			// Toggle bounding boxes 
			if (Zenith::Input::IsKeyPressed(ZN_KEY_LEFT_CONTROL))
			{
				m_UIShowBoundingBoxes = !m_UIShowBoundingBoxes;
				ShowBoundingBoxes(m_UIShowBoundingBoxes, m_UIShowBoundingBoxesOnTop);
			}
			break;
		}
		return false;
	}
private:
	Zenith::Scope<Zenith::SceneHierarchyPanel> m_SceneHierarchyPanel;

	Zenith::Ref<Zenith::Scene> m_Scene;
	Zenith::Ref<Zenith::Scene> m_SphereScene;
	Zenith::Ref<Zenith::Scene> m_ActiveScene;

	Zenith::Entity* m_MeshEntity = nullptr;

	Zenith::Ref<Zenith::Shader> m_BrushShader;
	Zenith::Ref<Zenith::Mesh> m_PlaneMesh;
	Zenith::Ref<Zenith::Material> m_SphereBaseMaterial;

	Zenith::Ref<Zenith::Material> m_MeshMaterial;
	std::vector<Zenith::Ref<Zenith::MaterialInstance>> m_MetalSphereMaterialInstances;
	std::vector<Zenith::Ref<Zenith::MaterialInstance>> m_DielectricSphereMaterialInstances;

	float m_GridScale = 16.025f, m_GridSize = 0.025f;
	float m_MeshScale = 1.0f;

	struct AlbedoInput
	{
		glm::vec3 Color = { 0.972f, 0.96f, 0.915f }; // Silver, from https://docs.unrealengine.com/en-us/Engine/Rendering/Materials/PhysicallyBased
		Zenith::Ref<Zenith::Texture2D> TextureMap;
		bool SRGB = true;
		bool UseTexture = false;
	};
	AlbedoInput m_AlbedoInput;

	struct NormalInput
	{
		Zenith::Ref<Zenith::Texture2D> TextureMap;
		bool UseTexture = false;
	};
	NormalInput m_NormalInput;

	struct MetalnessInput
	{
		float Value = 1.0f;
		Zenith::Ref<Zenith::Texture2D> TextureMap;
		bool UseTexture = false;
	};
	MetalnessInput m_MetalnessInput;

	struct RoughnessInput
	{
		float Value = 0.3f;
		Zenith::Ref<Zenith::Texture2D> TextureMap;
		bool UseTexture = false;
	};
	RoughnessInput m_RoughnessInput;

	struct Light
	{
		glm::vec3 Direction;
		glm::vec3 Radiance;
	};
	Light m_Light;
	float m_LightMultiplier = 0.3f;

	// PBR params
	bool m_RadiancePrefilter = false;

	float m_EnvMapRotation = 0.0f;

	enum class SceneType : uint32_t
	{
		Spheres = 0, Model = 1
	};
	SceneType m_SceneType;

	// Editor resources
	Zenith::Ref<Zenith::Texture2D> m_CheckerboardTex;

	int m_GizmoType = -1; // -1 = no gizmo
	bool m_AllowViewportCameraEvents = false;
	bool m_DrawOnTopBoundingBoxes = false;

	bool m_UIShowBoundingBoxes = false;
	bool m_UIShowBoundingBoxesOnTop = false;
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
		PushLayer(new EditorLayer());
	}
};

Zenith::Application* Zenith::CreateApplication(int arc, char** argv)
{
	return new Sandbox();
}