class SpecFuncs
{
    ICAppLayer* appLayer;
    
    public:
     SpecFuncs(ICAppLayer *appl);
    ~SpecFuncs();

     errType StartSpecFuncs();
     errType StopSpecFuncs();
};
