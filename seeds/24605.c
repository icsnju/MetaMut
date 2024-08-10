/* { dg-do compile } */
/* { dg-options "-fanalyzer" } */
/* { dg-require-effective-target analyzer } */

/* Adapted from CPython 3.8's ceval.h.  */
typedef struct PyThreadState PyThreadState;
extern PyThreadState * PyEval_SaveThread(void);
extern void PyEval_RestoreThread(PyThreadState *);

#define Py_BEGIN_ALLOW_THREADS { \
                        PyThreadState *_save; \
                        _save = PyEval_SaveThread();
#define Py_BLOCK_THREADS        PyEval_RestoreThread(_save);
#define Py_UNBLOCK_THREADS      _save = PyEval_SaveThread();
#define Py_END_ALLOW_THREADS    PyEval_RestoreThread(_save); \
                 }

/* Adapted/hacked up from CPython 3.8's object.h.  */

typedef struct _object {
    int ob_refcnt;
} PyObject;

#define _PyObject_CAST(op) ((PyObject*)(op))

extern void _Py_Dealloc(PyObject *);

#define _Py_INCREF(OP) do { (OP)->ob_refcnt++; } while (0);
#define _Py_DECREF(OP) do {	  \
    if (--(OP)->ob_refcnt == 0) { \
      _Py_Dealloc(OP);		  \
    }				  \
  } while (0)

#define Py_INCREF(op) _Py_INCREF(_PyObject_CAST(op))
#define Py_DECREF(op) _Py_DECREF(_PyObject_CAST(op))

void test_1 (void)
{
  Py_BEGIN_ALLOW_THREADS
  Py_END_ALLOW_THREADS
}

void test_2 (PyObject *obj)
{
  Py_BEGIN_ALLOW_THREADS /* { dg-message "releasing the GIL here" } */

  Py_INCREF (obj); /* { dg-warning "use of PyObject '\\*obj' without the GIL" } */
  Py_DECREF (obj);

  Py_END_ALLOW_THREADS
}

void test_3 (PyObject *obj)
{
  Py_BEGIN_ALLOW_THREADS /* { dg-message "releasing the GIL here" } */

  Py_BEGIN_ALLOW_THREADS /* { dg-warning "nested usage of 'Py_BEGIN_ALLOW_THREADS'" } */
  Py_END_ALLOW_THREADS

  Py_END_ALLOW_THREADS
}

void test_4 (PyObject *obj)
{
  /* These aren't nested, so should be OK.  */
  Py_BEGIN_ALLOW_THREADS
  Py_END_ALLOW_THREADS

  Py_BEGIN_ALLOW_THREADS
  Py_END_ALLOW_THREADS
}

/* Interprocedural example of erroneously nested usage.  */

static void  __attribute__((noinline))
called_by_test_5 (void)
{
  Py_BEGIN_ALLOW_THREADS /* { dg-warning "nested usage of 'Py_BEGIN_ALLOW_THREADS'" } */
  Py_END_ALLOW_THREADS
}

void test_5 (PyObject *obj)
{
  Py_BEGIN_ALLOW_THREADS /* { dg-message "releasing the GIL here" } */
  called_by_test_5 ();
  Py_END_ALLOW_THREADS
}

/* Interprocedural example of bogusly using a PyObject outside of GIL.  */

static void  __attribute__((noinline))
called_by_test_6 (PyObject *obj)
{
  Py_INCREF (obj); /* { dg-warning "use of PyObject '\\*obj' without the GIL" } */
  Py_DECREF (obj);
}

void test_6 (PyObject *obj)
{
  Py_BEGIN_ALLOW_THREADS /* { dg-message "releasing the GIL here" } */
  called_by_test_6 (obj);
  Py_END_ALLOW_THREADS
}

extern void called_by_test_7 (PyObject *obj);

void test_7 (PyObject *obj)
{
  Py_BEGIN_ALLOW_THREADS /* { dg-message "releasing the GIL here" } */
  called_by_test_7 (obj); /* { dg-warning "use of PyObject as argument 1 of 'called_by_test_7' without the GIL" } */
  Py_END_ALLOW_THREADS
}

typedef void (*callback_t) (PyObject *);

void test_8 (PyObject *obj, callback_t cb)
{
  Py_BEGIN_ALLOW_THREADS /* { dg-message "releasing the GIL here" } */
  cb (obj); /* { dg-warning "use of PyObject as argument 1 of call without the GIL" } */
  Py_END_ALLOW_THREADS
}
