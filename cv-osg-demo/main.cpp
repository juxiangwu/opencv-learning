#include <osgGA/TrackballManipulator>

#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgDB/ReadFile>
#include <opencv2/opencv.hpp>

static const char gVertexShader[] =
        "varying vec2 v_texCoord;                                         \n"
        "void main() {                                                    \n"
        "    gl_Position  = gl_ModelViewProjectionMatrix * gl_Vertex;     \n"
        "    v_texCoord   = gl_MultiTexCoord0.xy;                           \n"
        "}                                                                \n";

static const char gFragmentShader[] =
        "varying  mediump vec2 v_texCoord;                      \n"
        "uniform sampler2D sam;                                 \n"
        "void main() {                                          \n"
        "    gl_FragColor = texture2D(sam, v_texCoord);         \n"
        "}                                                      \n";


//创建一个四边形节点
osg::ref_ptr<osg::Node>  createNode() {

    //创建一个叶节点对象
    osg::ref_ptr<osg::Geode> geode = new osg::Geode();
    //创建一个几何体对象
    osg::ref_ptr<osg::Geometry> geom = new osg::Geometry();
    //添加顶点数据 注意顶点的添加顺序是逆时针
    osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();
    //添加数据
    v->push_back(osg::Vec3(0, 0, 0));
    v->push_back(osg::Vec3(1, 0, 0));
    v->push_back(osg::Vec3(1, 0, 1));
    v->push_back(osg::Vec3(0, 0, 1));

    //设置顶点数据
    geom->setVertexArray(v.get());

    //创建纹理订点数据
    osg::ref_ptr<osg::Vec2Array> vt = new osg::Vec2Array();
    //添加纹理坐标
    vt->push_back(osg::Vec2(0, 0));
    vt->push_back(osg::Vec2(1, 0));
    vt->push_back(osg::Vec2(1, 1));
    vt->push_back(osg::Vec2(0, 1));

    //设置纹理坐标
    geom->setTexCoordArray(0, vt.get());

    //创建颜色数组
    osg::ref_ptr<osg::Vec4Array> vc = new osg::Vec4Array();
    //添加数据
    vc->push_back(osg::Vec4(1, 0, 0, 1));
    vc->push_back(osg::Vec4(0, 1, 0, 1));
    vc->push_back(osg::Vec4(0, 0, 1, 1));
    vc->push_back(osg::Vec4(1, 1, 0, 1));

    //设置颜色数组
    //geom->setColorArray(vc.get());
    //设置颜色的绑定方式为单个顶点
    //geom->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
    //创建法线数组
    osg::ref_ptr<osg::Vec3Array> nc = new osg::Vec3Array();
    //添加法线
    nc->push_back(osg::Vec3(0, -1, 0));
    //设置法线
    geom->setNormalArray(nc.get());
    //设置法绑定为全部顶点
    geom->setNormalBinding(osg::Geometry::BIND_OVERALL);
    //添加图元
    geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS, 0, 4));

    //添加到叶子节点
    geode->addDrawable(geom.get());
    osg::StateSet* stateset = new osg::StateSet;
    osg::Texture2D* texture = new osg::Texture2D();
    texture->setDataVariance(osg::Object::DYNAMIC);
    cv::Mat src = cv::imread("/home/jenson/Pictures/linear.jpg");
    std::cout << src.size() << std::endl;
    //cv::imshow("src",src);
    cv::Mat dst;
    cv::cvtColor(src,dst,cv::COLOR_BGR2RGB);
    osg::Image * image = new osg::Image();
    //image->allocateImage(src.cols,src.rows,1,GL_RGB,GL_UNSIGNED_BYTE);
    image->setImage(dst.cols,dst.rows,3,
                  GL_RGB,GL_RGB,GL_UNSIGNED_BYTE,&dst.data[0],
                  osg::Image::NO_DELETE,1);
    //image->readPixels(0,0,src.cols,src.rows,1,GL_RGB,GL_UNSIGNED_BYTE);

    //texture->setImage(osgDB::readImageFile("/home/jenson/Pictures/linear.jpg"));
    texture->setImage(image);
    osg::Uniform* samUniform = new osg::Uniform(osg::Uniform::SAMPLER_2D, "sam");
    samUniform->set(0);//设置纹理单元
    stateset->addUniform(samUniform);
    stateset->setTextureAttributeAndModes(0, texture, osg::StateAttribute::ON);

    geode->setStateSet(stateset);

    return geode.get();
}

int main(int argc,char * argv[]){
    osg::ArgumentParser arguments(&argc,argv);

    // Create viewer
    osgViewer::Viewer viewer(arguments);
    viewer.setUpViewInWindow(0, 0, 640, 480);

    osg::Group* root = new osg::Group();
    osg::ref_ptr<osg::Node> node = createNode();
    root->addChild(node.get());
    viewer.setSceneData(root);
    viewer.realize();
    viewer.run();
    return 0;
}
