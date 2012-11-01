#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <tr1/functional>
//#include <boost/container/vector.hpp>
#include <vector>
//using namespace boost::container;
using namespace std;
#include "ply_parser.hpp"
#include "glm/glm.hpp"

using namespace std::tr1::placeholders;
template <typename TVertex, typename TIndex>
class Ply
{
public:
  Ply(TVertex& avertices, TIndex& aindices);
  bool parse(const string& istream_filename);

  std::size_t totalIndex;
  std::size_t totalVertex;

private:
  void info_callback(const std::string& filename, std::size_t line_number, const std::string& message);
  void warning_callback(const std::string& filename, std::size_t line_number, const std::string& message);
  void error_callback(const std::string& filename, std::size_t line_number, const std::string& message);
  std::tr1::tuple<std::tr1::function<void()>, std::tr1::function<void()> > element_definition_callback(const std::string& element_name, std::size_t count);
  template <typename ScalarType> std::tr1::function<void (ScalarType)> scalar_property_definition_callback(const std::string& element_name, const std::string& property_name);
  template <typename SizeType, typename ScalarType> std::tr1::tuple<std::tr1::function<void (SizeType)>, std::tr1::function<void (ScalarType)>, std::tr1::function<void ()> > list_property_definition_callback(const std::string& element_name, const std::string& property_name);
  void vertex_begin();
  void vertex_x(ply::float32 x);
  void vertex_y(ply::float32 y);
  void vertex_z(ply::float32 z);
  void vertex_end();
  void face_begin();
  void face_vertex_indices_begin(ply::uint8 size);
  void face_vertex_indices_element(ply::int32 vertex_index);
  void face_vertex_indices_end();
  void face_end();
  float vertex_x_, vertex_y_, vertex_z_;
  TVertex& vertices;
  TIndex& indices;
  std::size_t curIndex;
  std::size_t curVertex;
  std::string istream_filename;
};

template <typename TVertex, typename TIndex>
Ply<TVertex,TIndex>::Ply(TVertex& avertices, TIndex& aindices)
  :vertices(avertices), indices(aindices)
{
    curVertex=0;
    curIndex=0;
}

template <typename TVertex, typename TIndex>
void Ply<TVertex,TIndex>::info_callback(const std::string& filename, std::size_t line_number, const std::string& message)
{
  std::cerr << filename << ":" << line_number << ": " << "info: " << message << std::endl;
}
template <typename TVertex, typename TIndex>
void Ply<TVertex,TIndex>::warning_callback(const std::string& filename, std::size_t line_number, const std::string& message)
{
  std::cerr << filename << ":" << line_number << ": " << "warning: " << message << std::endl;
}

template <typename TVertex, typename TIndex>
void Ply<TVertex,TIndex>::error_callback(const std::string& filename, std::size_t line_number, const std::string& message)
{
  std::cerr << filename << ":" << line_number << ": " << "error: " << message << std::endl;
}

template <typename TVertex, typename TIndex>
std::tr1::tuple<std::tr1::function<void()>, std::tr1::function<void()> > Ply<TVertex,TIndex>::element_definition_callback(const std::string& element_name, std::size_t count)
{
  if (element_name == "vertex") {
      totalVertex=count;
      vertices.resize(count);
      return std::tr1::tuple<std::tr1::function<void()>, std::tr1::function<void()> >(
      std::tr1::bind(&Ply::vertex_begin, this),
      std::tr1::bind(&Ply::vertex_end, this)
    );
  }
  else if (element_name == "face") {
      totalIndex=count*3;
      indices.resize(totalIndex);
      return std::tr1::tuple<std::tr1::function<void()>, std::tr1::function<void()> >(
      std::tr1::bind(&Ply::face_begin, this),
      std::tr1::bind(&Ply::face_end, this)
    );
  }
  else {
    return std::tr1::tuple<std::tr1::function<void()>, std::tr1::function<void()> >(0, 0);
  }
}

template <typename TVertex, typename TIndex>
template <typename ScalarType>
std::tr1::function<void (ScalarType)> Ply<TVertex,TIndex>::scalar_property_definition_callback(const std::string& element_name, const std::string& property_name)
{
  if (element_name == "vertex") {
    if (property_name == "x") {
      return std::tr1::bind(&Ply::vertex_x, this, _1);
    }
    else if (property_name == "y") {
      return std::tr1::bind(&Ply::vertex_y, this, _1);
    }
    else if (property_name == "z") {
      return std::tr1::bind(&Ply::vertex_z, this, _1);
    }
    else {
      return 0;
    }
  }
  else {
    return 0;
  }
}

template <typename TVertex, typename TIndex>
template <class SizeType, class ScalarType>
std::tr1::tuple<std::tr1::function<void (SizeType)>, std::tr1::function<void (ScalarType)>, std::tr1::function<void ()> > Ply<TVertex,TIndex>::list_property_definition_callback(const std::string& element_name, const std::string& property_name)
{
  if ((element_name == "face") && (property_name == "vertex_indices")) {
    return std::tr1::tuple<std::tr1::function<void (ply::uint8)>, std::tr1::function<void (ply::int32)>, std::tr1::function<void ()> >(
      std::tr1::bind(&Ply::face_vertex_indices_begin, this, _1),
      std::tr1::bind(&Ply::face_vertex_indices_element, this, _1),
      std::tr1::bind(&Ply::face_vertex_indices_end, this)
    );
  }
  else {
    return std::tr1::tuple<std::tr1::function<void (ply::uint8)>, std::tr1::function<void (ply::int32)>, std::tr1::function<void ()> >(0, 0, 0);
  }
}

template <typename TVertex, typename TIndex>
void Ply<TVertex,TIndex>::vertex_begin()
{

}

template <typename TVertex, typename TIndex>
void Ply<TVertex,TIndex>::vertex_x(ply::float32 x)
{
  vertex_x_ = x;
}

template <typename TVertex, typename TIndex>
void Ply<TVertex,TIndex>::vertex_y(ply::float32 y)
{
  vertex_y_ = y;
}

template <typename TVertex, typename TIndex>
void Ply<TVertex,TIndex>::vertex_z(ply::float32 z)
{
  vertex_z_ = z;
}

template <typename TVertex, typename TIndex>
void Ply<TVertex,TIndex>::vertex_end()
{
    if(curVertex >= totalVertex){
        std::cerr<<"Uncorrect number of vertices in file:"
                <<istream_filename<<std::endl;
    }
    vertices[curVertex++]=typename TVertex::value_type(vertex_x_, vertex_y_, vertex_z_);
}

template <typename TVertex, typename TIndex>
void Ply<TVertex,TIndex>::face_begin()
{

}

template <typename TVertex, typename TIndex>
void Ply<TVertex,TIndex>::face_vertex_indices_begin(ply::uint8 size)
{
    assert(size==3);
}

template <typename TVertex, typename TIndex>
void Ply<TVertex,TIndex>::face_vertex_indices_element(ply::int32 vertex_index)
{
    indices[curIndex++]=vertex_index;
}

template <typename TVertex, typename TIndex>
void Ply<TVertex,TIndex>::face_vertex_indices_end()
{

}

template <typename TVertex, typename TIndex>
void Ply<TVertex,TIndex>::face_end()
{
}

template <typename TVertex, typename TIndex>
bool Ply<TVertex,TIndex>::parse(const string& filename)
{
  std::ifstream ifstream;
  istream_filename=filename;
  ifstream.open(istream_filename.c_str(),std::ifstream::in);
  if (!ifstream.is_open()) {
    std::cerr << "can not open file " << istream_filename << ": " << "no such file or directory" << "\n";
    exit(EXIT_FAILURE);
  }
  ply::ply_parser ply_parser;

  ply_parser.info_callback(std::tr1::bind(&Ply<TVertex,TIndex>::info_callback, this, std::tr1::ref(istream_filename), _1, _2));
  ply_parser.warning_callback(std::tr1::bind(&Ply<TVertex,TIndex>::warning_callback, this, std::tr1::ref(istream_filename), _1, _2));
  ply_parser.error_callback(std::tr1::bind(&Ply<TVertex,TIndex>::error_callback, this, std::tr1::ref(istream_filename), _1, _2));

  ply_parser.element_definition_callback(std::tr1::bind(&Ply<TVertex,TIndex>::element_definition_callback, this, _1, _2));

  ply::ply_parser::scalar_property_definition_callbacks_type scalar_property_definition_callbacks;
  scalar_property_definition_callbacks.get<ply::float32>() = std::tr1::bind(&Ply<TVertex,TIndex>::scalar_property_definition_callback<ply::float32>, this, _1, _2);
  ply_parser.scalar_property_definition_callbacks(scalar_property_definition_callbacks);

  ply::ply_parser::list_property_definition_callbacks_type list_property_definition_callbacks;
  list_property_definition_callbacks.get<ply::uint8, ply::int32>() = std::tr1::bind(&Ply<TVertex,TIndex>::list_property_definition_callback<ply::uint8, ply::int32>, this, _1, _2);
  ply_parser.list_property_definition_callbacks(list_property_definition_callbacks);
  return ply_parser.parse(ifstream);
}
