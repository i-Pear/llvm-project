.. title:: clang-tidy - bugprone-invalid-argument-in-strerror

bugprone-invalid-argument-in-strerror
===============================================

TODO

Examples:

.. code-block:: c++

    strerror(-errno);

    strerror(-1);

    #define	EINVAL 22
    strerror(-EINVAL);
