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
    NODE_SET_PROTOTYPE_METHOD(s_ct, "proc", Proc);
    NODE_SET_PROTOTYPE_METHOD(s_ct, "process_events", Event);

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

  /*
   * new NodeTcl() -- allocate and return a new interpreter.
   */
  static Handle<Value> New(const Arguments& args)
  {
    HandleScope scope;
    NodeTcl* hw = new NodeTcl();
    if (Tcl_Init(hw->m_interp) == TCL_ERROR) {
      Local<String> err = String::New(Tcl_GetStringResult(hw->m_interp));
      delete hw;
      return ThrowException(Exception::Error(err));
    }
    hw->Wrap(args.This());
    return args.This();
  }

  /*
   * eval(String) -- execute some Tcl code and return the result.
   */
  static Handle<Value> Eval(const Arguments& args)
  {
    HandleScope scope;

    if (args.Length() != 1 || !args[0]->IsString()) {
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


  /*
   * Data about a JavaScript function that can be executed.
   */
  struct callback_data_t {
    NodeTcl *hw;
    Tcl_Command cmd;
    Persistent<Function> jsfunc;
  };


  /*
   * Called by Tcl when it wants to invoke a JavaScript function.
   */
  static int CallbackTrampoline (ClientData clientData,
			     Tcl_Interp *interp,
			     int tclArgc,
			     const char *tclArgv[])
  {
    callback_data_t *cbdata = static_cast<callback_data_t*>(clientData);

    // Convert all of the arguments, but skip the 0th element (the procname).
    Local<Value> jsArgv[tclArgc - 1];
    for (int i = 1; i < tclArgc; i++) {
      jsArgv[i - 1] = String::New(tclArgv[i]);
    }

    // Execute the JavaScript method.
    TryCatch try_catch;
    Local<Value> result = cbdata->jsfunc->Call(Context::GetCurrent()->Global(), tclArgc - 1, jsArgv);

    // If a JavaScript exception occurred, send back a Tcl error.
    if (try_catch.HasCaught()) {
      Local<Message> msg = try_catch.Message();
      Tcl_SetObjResult(interp, Tcl_NewStringObj((const char*)*String::Utf8Value(msg->Get()), -1));
      return TCL_ERROR;
    }

    // Pass back to Tcl the returned value as a string.
    Tcl_SetObjResult(interp, Tcl_NewStringObj((const char*)*String::Utf8Value(result->ToString()), -1));
    return TCL_OK;
  }


  /*
   * proc(String, Function) -- define a proc within the Tcl namespace that executes a JavaScript function callback.
   */
  static Handle<Value> Proc(const Arguments& args)
  {
    HandleScope scope;

    if (args.Length() != 2) {
      return ThrowException(Exception::TypeError(String::New("Expecting 2 arguments (String, Function)")));
    }
    if (!args[0]->IsString()) {
      return ThrowException(Exception::TypeError(String::New("Argument 1 must be a string")));
    }
    Local<String> cmdName = Local<String>::Cast(args[0]);
    if (!args[1]->IsFunction()) {
      return ThrowException(Exception::TypeError(String::New("Argument 2 must be a function")));
    }
    Local<Function> jsfunc = Local<Function>::Cast(args[1]);

    NodeTcl* hw = ObjectWrap::Unwrap<NodeTcl>(args.This());

    callback_data_t *cbdata = new callback_data_t();
    cbdata->hw = hw;
    cbdata->jsfunc = Persistent<Function>::New(jsfunc);
    cbdata->cmd = Tcl_CreateCommand(hw->m_interp, (const char*)*String::Utf8Value(cmdName), CallbackTrampoline, (ClientData) cbdata, NULL);

    hw->Ref();

    return Undefined();
  }

  /*
   * event(Boolean) -- handle one or all pending Tcl events if boolean is present and true
   */
  static Handle<Value> Event(const Arguments& args)
  {
    HandleScope scope;
    int eventStatus;
    int doMultiple;

    if (args.Length() > 1 || (args.Length() == 1 && !args[0]->IsBoolean())) {
      return ThrowException(Exception::TypeError(String::New("Optional argument, if present, must be a boolean")));
    }

    if (args.Length() == 0)  {
	doMultiple = 1;
    } else {
	doMultiple = args[0]->ToBoolean()->Value();
    }

    do {
	eventStatus = Tcl_DoOneEvent (TCL_ALL_EVENTS | TCL_DONT_WAIT);
    } while (doMultiple && eventStatus);

    Local<Integer> result = Integer::New(eventStatus);
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
