import java.util.Collections;
import java.util.HashMap;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

public class InventoryCatalogManager {

    private static final int THREAD_COUNT = 10;
    private static final int OPERATIONS_PER_THREAD = 50000;
    private static final String PRODUCT_KEY = "PROD_LAPTOP_01";
