#include <Python.h>
#include "serial/qcdm_serial.h"

typedef struct {
	PyObject_HEAD
		QcdmSerial* port;
} PyQcdmSerial;

static PyTypeObject QcdmSerialType = {
	PyObject_HEAD_INIT(NULL)
	0,						/* ob_size        */
	"openpst.QcdmSerial",	/* tp_name        */
	sizeof(PyQcdmSerial),			/* tp_basicsize   */
	0,				/* tp_itemsize    */
	qcdm_serial_dealloc,				/* tp_dealloc     */
	0,				/* tp_print       */
	0,				/* tp_getattr     */
	0,				/* tp_setattr     */
	0,				/* tp_compare     */
	0,				/* tp_repr        */
	0,				/* tp_as_number   */
	0,				/* tp_as_sequence */
	0,				/* tp_as_mapping  */
	0,				/* tp_hash        */
	0,				/* tp_call        */
	0,				/* tp_str         */
	0,				/* tp_getattro    */
	0,				/* tp_setattro    */
	0,				/* tp_as_buffer   */
	Py_TPFLAGS_DEFAULT,				/* tp_flags       */
	"OpenPST QCDM Python Module",	/* tp_doc         */
};

PyMethodDef counter_methods[] = {
	{ "sendSpc", qcdm_serial_send_spc, METH_VARARGS },
	{ "sendPassword", qcdm_serial_send_password, METH_VARARGS },
	{ "setPhoneMode", qcdm_serial_set_phone_mode, METH_VARARGS },
	{ "getNvItem", qcdm_serial_get_nv_item, METH_VARARGS },
	{ "setNvItem", qcdm_serial_set_nv_item, METH_VARARGS },
	{ NULL, NULL },
};

PyMODINIT_FUNC initqcdm(void);

PyObject* qcdm_serial_NEW(port) const char* port;
PyObject* qcdm_serial_new(self, args) PyObject *self, *args;
void qcdm_serial_dealloc(self) PyObject *self;

PyObject * qcdm_serial_send_spc(self, args) PyObject *self, *args;
PyObject * qcdm_serial_send_password(self, args) PyObject *self, *args;
PyObject * qcdm_serial_set_phone_mode(self, args) PyObject *self, *args;
PyObject * qcdm_serial_get_nv_item(self, args) PyObject *self, *args;
PyObject * qcdm_serial_set_nv_item(self, args) PyObject *self, *args;
