#include <devices/dslr.hh>
#include <fcntl.h>

#include <iostream>
#include <sstream>

using namespace std;

GPContext* Dslr::s_ctx = NULL;

Dslr::Dslr() : m_cam(NULL), m_name(""), m_port("")
{
    if (!s_ctx)
        s_ctx = gp_context_new();

    // Initialize camera first camera
    gp_camera_new(&m_cam);
    int ret = gp_camera_init(m_cam, s_ctx);
    checkAndThrow(ret, "Failed to initialise camera");

    // Get camera port
    GPPortInfo info;
    char *tmp;
    ret = gp_camera_get_port_info(m_cam, &info);
    checkAndThrow(ret, "Failed to get port info");
    ret = gp_port_info_get_path(info, &tmp);
    checkAndThrow(ret, "Failed to get path");
    m_port = string(tmp);

    // Get camera name
    CameraAbilities abilities;
    ret = gp_camera_get_abilities(m_cam, &abilities);
    checkAndThrow(ret, "Failed to get camera abilities");
    m_name = string(abilities.model);

    return;
}

Dslr::Dslr(string port) : m_cam(NULL), m_name(""), m_port("")
{
    if (!s_ctx)
        s_ctx = gp_context_new();

    gp_camera_new(&m_cam);

    // Set camera name and port
    m_port = port;
    m_name = this->getCameraName(m_port);

    // Get abities of model
    CameraAbilities abilities;
    CameraAbilitiesList *ab_list;
    int ret = gp_abilities_list_new(&ab_list);
    checkAndThrow(ret, "Failed to intialise abilities list");
    ret = gp_abilities_list_load(ab_list, s_ctx);
    checkAndThrow(ret, "Failed to load abilities list");
    int idx = gp_abilities_list_lookup_model(ab_list, m_name.c_str());
    checkAndThrow(idx, "Model '" + m_name + "' not found!");
    ret = gp_abilities_list_get_abilities(ab_list, idx, &abilities);
    checkAndThrow(ret, "Failed to get camera abilities");
    // Finally: copy abilities to camera ptr
    ret = gp_camera_set_abilities(m_cam, abilities);
    checkAndThrow(ret, "Failed to copy camera abilities");

    // Get port info of the model
    GPPortInfo port_info;
    GPPortInfoList* p_list;
    ret = gp_port_info_list_new(&p_list);
    checkAndThrow(ret, "Failed to intialise port info list");
    ret = gp_port_info_list_load(p_list);
    checkAndThrow(ret, "Failed to load port info list");
    idx = gp_port_info_list_lookup_path(p_list, m_port.c_str());
    checkAndThrow(idx, "Port '" + m_port + "' not found!");
    ret = gp_port_info_list_get_info(p_list, idx, &port_info);
    checkAndThrow(ret, "Failed to get camera port info");
    // Finally: copy port info to camera ptr
    ret = gp_camera_set_port_info(m_cam, port_info);
    checkAndThrow(ret, "Failed to copy camera port info");

    // Initialize camera
    ret = gp_camera_init(m_cam, s_ctx);
    checkAndThrow(ret, "Failed to initialise camera");

    // Cleanup
    gp_abilities_list_free(ab_list);
    gp_port_info_list_free(p_list);

    return;
}

Dslr::~Dslr()
{
    gp_camera_exit(m_cam, s_ctx);
    gp_camera_free(m_cam);
    gp_context_unref(s_ctx);
    return;
}

void Dslr::captureToFile(std::string path)
{
    // Capture image and get file path on camera
    CameraFilePath cam_path;
    int ret = gp_camera_capture(m_cam, GP_CAPTURE_IMAGE, &cam_path, s_ctx);
    checkAndThrow(ret, "Failed to capture image");
    
    // Associate file descriptor with camera file
    CameraFile *cam_file;
    int fd = open(path.c_str(), O_CREAT | O_WRONLY, 0644);
    ret = gp_file_new_from_fd(&cam_file, fd);
    checkAndThrow(ret, "Failed to open fd");

    // Copy file from camera to disk
    ret = gp_camera_file_get(m_cam, cam_path.folder, cam_path.name, 
        GP_FILE_TYPE_NORMAL, cam_file, s_ctx);
    checkAndThrow(ret, "Failed to copy file from camera");
    
    // Remove file from camera
    ret = gp_camera_file_delete(m_cam, cam_path.folder, cam_path.name, s_ctx);
    checkAndThrow(ret, "Failed to delete file from camera");
    gp_file_free(cam_file);

    return;
}

unsigned long Dslr::captureRaw(const char *raw_data)
{
    // Capture image and get file path on camera
    CameraFilePath cam_path;
    int ret = gp_camera_capture(m_cam, GP_CAPTURE_IMAGE, &cam_path, s_ctx);
    checkAndThrow(ret, "Failed to capture image");

    // Copy file from camera to disk
    CameraFile *cam_file;
    ret = gp_file_new(&cam_file);
    checkAndThrow(ret, "Failed to create new camera file");
    ret = gp_camera_file_get(m_cam, cam_path.folder, cam_path.name, 
        GP_FILE_TYPE_NORMAL, cam_file, s_ctx);
    checkAndThrow(ret, "Failed to copy file from camera");

    unsigned long size;
    gp_file_get_data_and_size(cam_file, &raw_data, &size);

    // Remove file from camera
    ret = gp_camera_file_delete(m_cam, cam_path.folder, cam_path.name, s_ctx);
    checkAndThrow(ret, "Failed to delete file from camera");
    gp_file_free(cam_file);

    return size;

}


unsigned Dslr::getDslrList(std::vector<std::string> &name_list,
    std::vector<std::string> &port_list)
{
    CameraList *list;

    GPContext *ctx = gp_context_new();
    int ret = gp_list_new(&list);
    checkAndThrow(ret, "Failed to get camera list");

    int n = gp_camera_autodetect(list, ctx);
    checkAndThrow(ret, "No cameras detected");

    const char *name, *port;
    for (int i = 0; i < n; i++) 
    {
        ret = gp_list_get_name(list, i, &name);
        checkAndThrow(ret, "Failed to get name");

        ret = gp_list_get_value(list, i, &port);
        checkAndThrow(ret, "Failed to get port");

        name_list.push_back(name);
        port_list.push_back(port);
    }

    gp_list_free(list);
    gp_context_unref(ctx);
    
    return static_cast<unsigned>(n);
}

/*
 *      P R I V A T E   M E T H O D S
 */

string Dslr::getCameraName(string port)
{
    // Get list of connected cameras
    CameraList *list;
    int ret = gp_list_new(&list);
    checkAndThrow(ret, "Failed to get camera list");
    int n = gp_camera_autodetect(list, s_ctx);
    checkAndThrow(ret, "No cameras detected");

    const char *name, *value;
    string model_name("");
    for (int i = 0; i < n; i++) 
    {
        ret = gp_list_get_name(list, i, &name);
        checkAndThrow(ret, "Failed to get name");

        ret = gp_list_get_value(list, i, &value);
        checkAndThrow(ret, "Failed to get port");

        if ( string(value) == port ) 
        {
            cout << "Found port '" << value << "'";
            model_name = string(name);
            cout << " -> Model: " << model_name << endl;
            break;
        }
    }

    gp_list_free(list);

    if (model_name.empty())
        throw DslrException("Did not find camera on port '" + port + "'");

    return model_name;
}

void Dslr::checkAndThrow(int ret, std::string err_msg)
{
    if (ret < GP_OK) 
    {
        stringstream ss("");
        ss << err_msg << " - " << gp_port_result_as_string(ret) << " ";
        ss << "(" << ret << ")";
        throw DslrException(ss.str(), ret);
    }

    return;
}
