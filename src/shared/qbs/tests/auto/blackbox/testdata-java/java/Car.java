class Car implements Vehicle
{
    private InternalCombustionEngine engine;

    public Car() {
        engine = new InternalCombustionEngine();
    }

    public void go()
    {
        System.out.println("Driving!");
        engine.run();
    }

    public class InternalCombustionEngine
    {
        public native void run();

        public class ChemicalReaction {
            public native void occur();
        }
    }
}
