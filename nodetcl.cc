#include <v8.h>
#include <node.h>
#include <tcl.h>


using namespace node;
using namespace v8;

class NodeTcl : ObjectWrap
{
private:
  Tcl_Interp *m_interp;
public:

  static Persistent<FunctionTemplate> s_ct;
  static void Init(Handle<Object> target)
  {
    HandleScope scope;

    Local<FunctionTemplate> t = FunctionTemplate::New(New);

    s_ct = Persistent<FunctionTemplate>::New(t);
    s_ct->InstanceTemplate()->SetInternalFieldCount(1);
    s_ct->SetClassName(String::NewSymbol("NodeTcl"));

    NODE_SET_PROTOTYPE_METHOD(s_ct, "eval", Eval);

    target->Set(String::NewSymbol("NodeTcl"), s_ct->GetFunction());
  }

  NodeTcl()
  {
    m_interp = Tcl_CreateInterp();
  }

  ~NodeTcl()
  {
    Tcl_DeleteInterp(m_interp);
    m_interp = NULL;
  }

  static Handle<Value> New(const Arguments& args)
  {
    HandleScope scope;
    NodeTcl* hw = new NodeTcl();
    hw->Wrap(args.This());
    return args.This();
  }

  static Handle<Value> Eval(const Arguments& args)
  {
    HandleScope scope;

    if (args.Length() <= 0 || !args[0]->IsString()) {
      return ThrowException(Exception::TypeError(String::New("Argument must be a string")));
    }

    Local<String> script = Local<String>::Cast(args[0]);

    NodeTcl* hw = ObjectWrap::Unwrap<NodeTcl>(args.This());
    
    int cc = Tcl_Eval(hw->m_interp, (const char*)*String::Utf8Value(script));
    if (cc != TCL_OK) {
      Tcl_Obj *obj = Tcl_GetObjResult(hw->m_interp);
      return ThrowException(Exception::Error(String::New(Tcl_GetString(obj))));
    }

    Tcl_Obj *obj = Tcl_GetObjResult(hw->m_interp);
    Local<String> result = String::New(Tcl_GetString(obj));
    return scope.Close(result);
  }

};

Persistent<FunctionTemplate> NodeTcl::s_ct;

extern "C" {
  static void init (Handle<Object> target)
  {
    NodeTcl::Init(target);
  }

  NODE_MODULE(nodetcl, init);
}
