#include <GUICreateProject.h>
#include <stdexcept>


GUICreateProject::GUICreateProject(){}


GUICreateProject::~GUICreateProject() {}

void GUICreateProject::render() {}
void GUICreateProject::destroy() {}
void GUICreateProject::start() {}



GUIView *GUICreateProject::create(Engine *_ptr){


	GUICreateProject *view = new GUICreateProject();


	view->_selfWindow = glfwCreateWindow(500, 250, "Start Project", nullptr, nullptr);
	if(!view)
		throw std::runtime_error("Problem initializing start project window");

	return dynamic_cast<GUIView *>(view);
	
}


