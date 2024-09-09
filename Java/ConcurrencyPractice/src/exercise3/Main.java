package exercise3;

public class Main {
    public static void main(String[] args) {

        //ex2:

//        MyMonitor mAtomicVar = new MonitorAtomicVar();
//        MyMonitor mSemaphores = new MonitorTwoSemaphores();
//
//        ProducerPong p1 = new ProducerPong(mAtomicVar);
//        ConsumerPing c1 = new ConsumerPing(mAtomicVar);
//
//
//        p1.start();
//        c1.start();



        //ex3:
//        MyMonitor listMonitor = new MonitorListSemaphore();
//
//        Producer[] producers = new Producer[2];
//        Consumer[] consumers = new Consumer[5];
//
//        for (int i = 0; i < 5; i++) {
//            consumers[i] = new Consumer(listMonitor);
//        }
//
//        for (int i = 0; i < 2; i++) {
//            producers[i] = new Producer(listMonitor);
//        }
//
//        for (int i = 0; i < 5; i++) {
//            consumers[i].start();
//        }
//
//        for (int i = 0; i < 2; i++) {
//            producers[i].start();
//        }
//
//        for (int i = 0; i < 2; i++) {
//            try {
//                producers[i].join();
//                consumers[i].join();
//            } catch (InterruptedException e) {
//                System.out.println("????");
//            }
//        }



        //barrier:
        MyMonitor barrierMonitor = new MonitorBarrier();

        Producer producer = new Producer(barrierMonitor);
        Consumer[] consumers = new Consumer[4];

        for (int i = 0; i < 4; i++) {
            consumers[i] = new Consumer(barrierMonitor);
        }

        producer.start();

        for (int i = 0; i < 4; i++) {
            consumers[i].start();
        }

        try {
            producer.join();
            for (int i = 0; i < 4; i++) {
                consumers[i].join();
            }
        } catch (InterruptedException e) {
            System.out.println("join interrupted");
            throw new RuntimeException(e);
        }


    }
}
