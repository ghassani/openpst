#include "qcdm.h"

PyMODINIT_FUNC initqcdm(void)
{
	PyObject* m;

	QcdmSerialType.tp_new = PyType_GenericNew;

	if (PyType_Ready(&QcdmSerialType) < 0) {
		return;
	}

	m = Py_InitModule3("simpletype", NULL, "Example module that creates an extension type.");
	
	if (m == NULL) {
		return;
	}
		
	Py_INCREF(&QcdmSerialType);

	PyModule_AddObject(m, "QcdmSerial", (PyObject *)&QcdmSerialType);
}

PyObject * qcdm_serial_NEW(port) const char* port;
{
	PyQcdmSerial *object = NULL;

	object = PyObject_NEW(PyQcdmSerial, &QcdmSerialType);
	
	if (object != NULL) {
		object->port = new QcdmSerial(port);
	}

	return (PyObject *)object;
}

void qcdm_serial_dealloc(self) PyObject *self;
{
	if (self->port->isOpen()) {
		self->port->close();
	}

	delete self->port;

	PyMem_DEL(self);
}


PyObject * qcdm_serial_new(self, args) PyObject *self, *args;
{
	PyObject *object = NULL;

	if (PyArg_ParseTuple(args, "|c", &port)) {
		object = qcdm_serial_NEW(port);
	}

	return object;
}

PyObject * qcdm_serial_send_spc(self, args) PyObject *self, *args;
{ 
	PyObject *result = NULL;
	PyQcdmSerial *object = (PyQcdmSerial *)self;

	if (PyArg_ParseTuple(args, "")) {  /* takes no args, other than self */
		object->value++;
		result = Py_None;
		Py_INCREF(result);
	}
	return result;
}


/*
PyObject *counter_getattr(self, attrname) PyObject *self; char *attrname;
{ 
	PyObject *result = NULL;
	PyQcdmSerial *object = (PyQcdmSerial *)self;

	if (strcmp(attrname, "value") == 0)
		result = PyInt_AsLong(object->value);
	else /* find and return a bound method */
		result = Py_FindMethod(counter_methods, self, attrname);
	return result;
}*/
