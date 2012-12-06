require 'libmemcached'
require 'test/unit'

class ClientTest < Test::Unit::TestCase
  def test_raises_config_error
    assert_raises Libmemcached::ConfigurationError do
      Libmemcached::NativeClient.new('abc')
    end
  end

  def test_invalid_server
    client = Libmemcached::NativeClient.new('--server=127.0.0.1:99999')
    assert_equal :connection_failure, client.set('foo', 'bar')
  end

  def test_bad_key
    client = local_client
    assert_equal :bad_key_provided, client.set('ffo'*300, 'bar')
  end

  def test_set_and_get
    client = local_client
    assert_equal true, client.set('foo', 'bar')
    assert_equal ['bar', 0], client.get('foo')
  end

  def test_set_and_get_with_flags
    client = local_client
    assert_equal true, client.set('foo', 'bar', 0, 123)
    assert_equal ['bar', 123], client.get('foo')
  end

  def test_mget
    values = {
      'foo1' => 'bar1',
      'foo2' => 'bar2',
      'foo3' => 'bar3'
    }

    client = local_client
    values.each{ |key, val| client.set(key, val) }
    values_with_flags = values.inject(Hash.new){ |h,(k,v)| h[k] = [v, 0]; h }
    assert_equal [:end, values_with_flags], client.mget(values.keys)
  end

  def test_delete
    client = local_client
    assert_equal true, client.set('foo', 'bar')
    assert_equal ['bar', 0], client.get('foo')
    assert_equal true, client.delete('foo')
    assert_equal false, client.delete('foo')
    assert_equal nil, client.get('foo')
  end

  def local_client
    Libmemcached::NativeClient.new('--server=127.0.0.1')
  end
end
