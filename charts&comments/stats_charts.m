%time, patientsInQueue, bloodInDepot, emergencyOrdersRatio, usedBloodRatio, avgTimeInQueue


figure
subplot(3,2,3)
plot(Stats(:,1), Stats(:,2) );
xlabel('time');
ylabel('patients in queue');

subplot(3,2,5)
plot(Stats(:,1), Stats(:,4) );
xlabel('time');
ylabel('time since emergency');

subplot(3,2,1)
plot(Stats(:,1), Stats(:,3) );
xlabel('time');
ylabel('blood units in depot');

subplot(3,2,6)
plot(Stats(:,1), Stats(:,5) );
xlabel('time');
ylabel('emergency order ratio');

subplot(3,2,2)
plot(Stats(:,1), Stats(:,6) );
xlabel('time');
ylabel('used blood units ratio');

subplot(3,2,4)
plot(Stats(:,1), Stats(:,7) );
xlabel('time');
ylabel('avg time in queue');