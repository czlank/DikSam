#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

extern int  g_iCurrentThreadIndex;

void dkc_function_define(DVM_BasicType type, char *identifier, ParameterList *parameter_list, Block *block)
{
    try
    {
        DikSam::GetClassObject(g_iCurrentThreadIndex)->GetCreate()->FunctionDefine(type, identifier, parameter_list, block);
    }
    catch (const ErrorException& e)
    {
        std::cout << e.what();
    }
    catch (const MemoryException& e)
    {
        std::cout << e.what();
    }
}

ParameterList* dkc_create_parameter(DVM_BasicType type, char *identifier)
{
    try
    {
        DikSam::GetClassObject(g_iCurrentThreadIndex)->GetCreate()->CreateParameter(type, identifier);
    }
    catch (const ErrorException& e)
    {
        std::cout << e.what();
    }
    catch (const MemoryException& e)
    {
        std::cout << e.what();
    }
}

#ifdef __cplusplus
}
#endif // __cplusplus